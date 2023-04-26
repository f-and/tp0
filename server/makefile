UNAME:=$(shell uname)

ifeq ($(UNAME),Linux)
FILE_EXT:=out
else
FILE_EXT:=exe
endif

SRC := src

FILES := ./$(SRC)/server.c ./$(SRC)/utils.c
OUTPUT_FILE := tp0

CC := gcc
CFLAGS := -I $(SRC) -lcommons -lreadline -Wall -Wextra -g

compile: title
	@echo "Compilando... "
	@$(CC) $(FILES) -o $(OUTPUT_FILE).$(FILE_EXT) $(CFLAGS)


title: 
	@echo "▀█▀ █▀█ █▀█"
	@echo "░█░ █▀▀ █▄█"
	
