## Opções de estrutura do projeto. ############################################
# Nome do programa principal.
NAME     = main

# Diretório onde se encontram as fontes (arquivos “.c”).
SRC_DIR  = src

# Diretório onde se encontram os cabeçalhos (arquivos “.h”).
INC_DIR  = include
CFLAGS  += -I$(INC_DIR)

# Diretório onde o compilador colocará o programa principal.
OUT_DIR  = build

# Diretório onde o compilador colocará os objetos compilados (arquivos “.o”).
OBJ_DIR  = $(OUT_DIR)/obj

# Diretório onde se encontrarão as figuras.
FIG_DIR  = $(OUT_DIR)/fig


## Opções do compilador. ######################################################
# Qual compilador usar.
CC       = gcc

# Opções que definem a versão de C, nível de otimização, e símbolos para debug.
CFLAGS  += -std=c23 -O3 -g

# Opções de diagnóstico.
CFLAGS  += -Wall -Wextra -pedantic

# Opções do linker (para incluir objetos compartilhados).
LDFLAGS +=

# Quais são as fontes, os objetos compilados, os cabeçalhos, e o programa
# principal.
SRCS     = $(wildcard $(SRC_DIR)/*.c)
OBJS     = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
HEADERS  = $(wildcard $(INC_DIR)/*.h)
BIN      = $(OUT_DIR)/$(NAME)
CSV      = $(BIN).csv


## Opções para cada plataforma, compilador, ferramenta, e configuração. #######
# Opções para compiladores específicos.

# Ative as opções de análise estática de código do GCC.
GCCCFLAGS   += --debug -fanalyzer
# Desative o aviso do Clang sobre falta de newline ao final do arquivo.
CLANGCFLAGS += --debug -Wno-newline-eof
# CLANGCFLAGS += -Xanalyzer ...

ifeq ($(CC),gcc)
    CFLAGS += $(GCCCFLAGS)
else ifeq ($(CC),clang)
    CFLAGS += $(CLANGCFLAGS)
else ifeq ($(CC),musl-gcc)
    CFLAGS  += $(GCCCFLAGS) -static
    LDFLAGS += -static
else ifeq ($(CC),musl-clang)
    CFLAGS  += $(CLANGCFLAGS) -Wno-unused-command-line-argument -static
    LDFLAGS += -static
endif

# Se estivermos no Windows, certos comandos devem ser diferentes.
ifneq ($(OS),Windows_NT)
    MKDIR = mkdir -p
    RMDIR = $(RM) -r
    is_installed = $(if $(shell which $1),installed,)
else
    MKDIR = mkdir
    RMDIR = rmdir /s /q
    is_installed = $(if $(shell where $1 2>NUL),installed,)
endif

# Se `compiledb` <https://github.com/nickdiego/compiledb> estiver instalado,
# defina essa veriável para que tenhamos diagnósticos corretos no editor que
# estiver usando `clangd`.
ifeq (installed,$(call is_installed,compiledb))
    CC_JSON = $(OUT_DIR)/compile_commands.json
endif

# Se `ccache` <https://ccache.dev> estiver instalado, modificamos a variável
# `CC` para habilitar caching de objetos compilados.
ifeq (installed,$(call is_installed,ccache))
    CC := ccache $(CC)
endif


## Regras para compilação. ####################################################
# Alvo padrão para compilar.
all: $(CC_JSON) $(BIN)

# Como montar o programa principal a partir dos objetos já compilados.
$(BIN): $(OBJS) $(HEADERS) | $(OUT_DIR)/
	$(CC) $(LDFLAGS) $(OBJS) -o $(BIN)

# Como compilar cada objeto a partir de sua fonte.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)/
	$(CC) $(CFLAGS) -o $@ -c $<

# Como criar `compile_commands.json` para diagnósticos. Aqui, não se pode usar
# a variável `MAKE`, pois causa recursão infinita em `compiledb`.
$(CC_JSON): Makefile | $(OUT_DIR)/
	compiledb -nfo $(CC_JSON) make CC=clang

# Como criar diretórios
%/:
	$(MKDIR) $@

# Como limpar o diretório, excluindo todos os artefatos da compilação.
clean:
	$(RM) $(BIN) $(OBJ) $(CC_JSON)
	$(RMDIR) $(OUT_DIR)
	$(MAKE) -C report clean

# Como criar o executável distributível.
release: $(BIN)-release docs

# Como criar executável sem símbolos de debugging.
$(BIN)-stripped: $(BIN)
	strip -o $(BIN)-stripped $(BIN)

# Como criar o executável comprimido.
$(BIN)-release: $(BIN)-stripped
	upx -qqo $(BIN)-upx $(BIN)-stripped
	mv -f $(BIN)-upx $(BIN)-release

# Como executar o programa principal.
run: $(BIN)
	@$<

# Como salvar os resultados num CSV.
$(CSV): $(BIN) | $(OUT_DIR)/
	$< 1 10000 > $@

csv: $(CSV)

# Como salvar as figuras.
$(FIG_DIR)/%.pdf: $(CSV) utils/plot.py | $(FIG_DIR)/
	python utils/plot.py

$(FIG_DIR)/%.tex: $(CSV) utils/tables.py | $(FIG_DIR)/
	python utils/tables.py

# Como salvar o relatório.
report/main.pdf: $(CSV) $(TBL) $(FIG_DIR)/*.pdf $(FIG_DIR)/*.tex report/*.tex
	$(MAKE) -C report

report: report/main.pdf


## Regras para gerar documentação. ############################################
# Gera a documentação usando Doxygen.
docs: $(OUT_DIR)/docs/html/index.html

$(OUT_DIR)/docs/html/index.html: docs/Doxyfile $(SRCS) $(HEADERS) | $(OUT_DIR)/
	doxygen docs/Doxyfile


## Alvos que não correspondem diretamente a arquivos ou diretórios. ###########
.PHONY: all clean release docs run csv report