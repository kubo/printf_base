CFLAGS = -Wall -g -I. -Isamples
TESTS = check_output check_output_on_linux check_dprintf check_snprintf
VPATH = samples test

all: $(TESTS)

check_output: check_output.o pfb_asprintf.o printf_base_dbl_wc.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

check_output_on_linux: check_output_on_linux.o pfb_asprintf.o printf_base_dbl_wc.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

check_dprintf: check_dprintf.c pfb_dprintf.o printf_base_dbl_wc.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

check_snprintf: check_snprintf.c pfb_snprintf.o printf_base_dbl_wc.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

test: $(TESTS)
	@for test in $(TESTS); do \
	  echo Run $$test...; \
	  ./$$test; \
	done

clean:
	$(RM) $(TESTS) *.o

printf_base.o: printf_base.c printf_base.h

printf_base_dbl_wc.o: printf_base.c printf_base.h
	$(CC) $(CFLAGS) -c -o $@ $< -DPRINTF_BASE_DOUBLE -DPRINTF_BASE_WIDE_CHAR

pfb_asprintf.o: pfb_asprintf.c pfb_asprintf.h printf_base.h
pfb_dprintf.o: pfb_dprintf.c pfb_dprintf.h printf_base.h
pfb_fprintf.o: pfb_fprintf.c pfb_fprintf.h printf_base.h
pfb_printf.o: pfb_printf.c pfb_printf.h printf_base.h
pfb_snprintf.o: pfb_snprintf.c pfb_snprintf.h printf_base.h

check_output_on_linux.o: check_output_on_linux.c pfb_asprintf.h printf_base.h
check_output.o: check_output.c pfb_asprintf.h printf_base.h

.PHONY: test clean
