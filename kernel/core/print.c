/* GPLv2 (c) Airbus */
#include <print.h>
#include <uart.h>
#include <string.h>
#include <asm.h>

static char vprint_buffer[1024];

void panic(const char *format, ...)
{
   va_list params;

   force_interrupts_off();

   va_start(params, format);
   __vprintf(format, params);
   va_end(params);

   uart_flush();
   while (1) halt();
}


size_t printf(const char *format, ...)
{
   va_list params;
   size_t  retval;

   va_start(params, format);
   retval = __vprintf(format, params);
   va_end(params);

   return retval;
}

size_t snprintf(char *buff, size_t len, const char *format, ...)
{
   va_list params;
   size_t  retval;

   va_start(params, format);
   retval = __vsnprintf(buff, len, format, params);
   va_end(params);

   return retval;
}

size_t __vprintf(const char *format, va_list params)
{
   size_t retval;

   retval = __vsnprintf(vprint_buffer,sizeof(vprint_buffer),format,params);
   uart_write((uint8_t*)vprint_buffer, retval-1);
   return retval;
}

static inline void __format_add_str(buffer_t *buf, size_t len, char *s)
{
   while(*s)
      __buf_add(buf, len, *s++);
}

static inline void __format_add_chr(buffer_t *buf, size_t len, int c)
{
   __buf_add(buf, len, (char)c);
}

static inline void __format_add_bin(buffer_t *buf, size_t len,
                                    uint64_t value, uint32_t n)
{
   uint32_t i, bit;

   for(i=0 ; i<n ; i++)
   {
      bit = (value >> (n-i-1)) & 1;
      __buf_add(buf, len, bit?'1':'0');
   }
}

static inline void __format_add_idec(buffer_t *buf, size_t len, sint64_t value)
{
   char     rep[24];
   buffer_t dec;

   if(!value)
      return __buf_add(buf, len, '0');

   dec.data.str = rep;
   dec.sz = 0;

   if(value < 0)
   {
      __buf_add(buf, len, '-');
      value = -value;
   }

   while(value)
   {
      dec.data.str[dec.sz++] = (value%10) + '0';
      value /= 10;
   }

   while(dec.sz--)
      __buf_add(buf, len, dec.data.str[dec.sz]);
}

static inline void __format_add_udec(buffer_t *buf, size_t len, uint64_t value)
{
   char     rep[24];
   buffer_t dec;

   if(!value)
      return __buf_add(buf, len, '0');

   dec.data.str = rep;
   dec.sz = 0;

   while(value)
   {
      dec.data.str[dec.sz++] = (value%10) + '0';
      value /= 10;
   }

   while(dec.sz--)
      __buf_add(buf, len, dec.data.str[dec.sz]);
}


static char __hextable[] = {'0','1','2','3','4','5','6','7',
                            '8','9','a','b','c','d','e','f'};

size_t uint64_to_hex(buffer_t *buf, size_t len,
                     uint64_t value, size_t precision)
{
   char   rep[sizeof(uint64_t)*2];
   size_t sz, rsz = 0;

   if(!precision || precision > 16)
      precision = -1;

   while(precision && !(precision > 16 && !value && rsz))
   {
      rep[rsz] = __hextable[value & 0xf];
      value >>= 4;
      rsz++;
      precision--;
   }

   sz = rsz;
   while(rsz--)
      __buf_add(buf, len, rep[rsz]);

   return sz;
}

static inline void __format_add_hex(buffer_t *buf, size_t len,
                                    uint64_t value, size_t precision)
{
   uint64_to_hex(buf, len, value, precision);
}

size_t __vsnprintf(char *buffer, size_t len,
                   const char *format, va_list params)
{
   buffer_t buf;
   size_t   size;
   char     c;
   bool_t   interp, lng;

   buf.data.str = buffer;
   buf.sz = 0;
   interp = false;
   lng = false;
   size = 4;

   if(len) len--;

   while(*format)
   {
      c = *format++;

      if(interp)
      {
         // length modifiers, may continue to keep 'interp'
         if(c == 'l'){
            if(lng)
               size = 8;
            else
               lng = true;
            continue;
         } else if(c == 'h'){
            size /= 2;
            continue;
         }

         // conversion modifiers
         if(c == 's'){
            char* value = va_arg(params, char*);
            __format_add_str(&buf, len, value);
         } else if(c == 'c'){
            int value = va_arg(params, int);
            __format_add_chr(&buf, len, value);
         } else if(c == 'b'){
            uint64_t value = va_arg(params, uint32_t);
            __format_add_bin(&buf, len, value, 32);
         } else if(c == 'B'){
            uint64_t value = va_arg(params, uint64_t);
            __format_add_bin(&buf, len, value, 64);

            // interpret size length modifier
         } else if(c == 'd' || c == 'i'){
            sint64_t value;

            if(size >= 8)
               value = va_arg(params, sint64_t);
            else if(size == 4)
               value = va_arg(params, sint32_t);
            else if(size == 2)
               value = (sint16_t)va_arg(params, int);
            else
               value = (sint8_t)va_arg(params, int);

            __format_add_idec(&buf, len, value);

         } else if(c == 'u' || c == 'x'){
            uint64_t value;

            if(size >= 8)
               value = va_arg(params, uint64_t);
            else if(size == 4)
               value = va_arg(params, uint32_t);
            else if(size == 2)
               value = (uint16_t)va_arg(params, unsigned int);
            else
               value = (uint8_t)va_arg(params, unsigned int);

            if(c == 'u')
               __format_add_udec(&buf, len, value);
            else
               __format_add_hex(&buf, len, value, 0);

            // force size to 64 bits
         } else if(c == 'D'){
            sint64_t value = va_arg(params, sint64_t);
            __format_add_idec(&buf, len, value);
         } else if(c == 'X'){
            uint64_t value = va_arg(params, uint64_t);
            __format_add_hex(&buf, len, value, 0);

            // '0x'%lx
         } else if(c == 'p'){
            uint64_t value = va_arg(params, uint32_t);
            __format_add_str(&buf, len, "0x");
            __format_add_hex(&buf, len, value, 0);

            // ignore padding, precision ...
         } else if (c >= '0' && c <= '9') {
            continue;

            // escaped '%'
         } else if (c == '%') {
            __buf_add(&buf, len, c);

            // take care of unsupported format used
         } else {
            panic("unsupported format arg '%c'\n", c);
         }

         interp = false;
         lng = false;
      }
      else if(c == '%')
      {
         interp = true;
         size = 4;
      }
      else
         __buf_add(&buf, len, c);
   }

   buf.data.str[buf.sz++] = 0;
   return buf.sz;
}
