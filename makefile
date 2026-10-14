# Formatação de código com clang-format
# Uso:
#   make format   -> formata todos os arquivos no lugar
#   make check    -> só verifica (falha se algo estiver fora do padrão)
#   make files    -> lista os arquivos que serão formatados

CLANG_FORMAT ?= clang-format
DIRS         := src include

SOURCES := $(shell find $(DIRS) -path '*/vendor' -prune -o \
	\( -name '*.cpp' -o -name '*.hpp' -o -name '*.h' \) -print)

.PHONY: format check files

format:
	@$(CLANG_FORMAT) -i $(SOURCES)
	@echo "$(words $(SOURCES)) formatted files."

check:
	@$(CLANG_FORMAT) --dry-run --Werror $(SOURCES)
	@echo "everything is fine."

files:
	@printf '%s\n' $(SOURCES)