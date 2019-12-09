INPUT_FILES		:=	$(wildcard *.S)
OUTPUT_FILES	:=	$(subst .S,.o,$(INPUT_FILES))

.PHONY: clean all

all: $(OUTPUT_FILES) .FORCE

%.o: %.S
	@[ -d thumb ] || mkdir -p thumb
	@$(CC) -x assembler-with-cpp -marm -c $< -o $@
	@$(CC) -x assembler-with-cpp -mthumb -c $< -o thumb/$@

clean:
	@rm -rf $(OUTPUT_FILES) thumb/

.FORCE:
