OUTPUT_FILES	:=	$(subst .S,.o,$(wildcard *.S))

.PHONY: clean all

all: $(OUTPUT_FILES) .FORCE

%.o: %.S
	@$(CC) -x assembler-with-cpp -marm -c $< -o $@

clean:
	@rm -rf $(OUTPUT_FILES)

.FORCE:
