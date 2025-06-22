CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -I../common
LDFLAGS = -lm

TARGET = s21_cat

SRC_DIR = .
COMMON_DIR = ../common

SRCS = $(SRC_DIR)/s21_cat.c $(COMMON_DIR)/s21_common.c
OBJS = $(SRC_DIR)/s21_cat.o $(SRC_DIR)/s21_common.o

all: $(SRC_DIR)/$(TARGET)

$(SRC_DIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(SRC_DIR)/s21_cat.o: $(SRC_DIR)/s21_cat.c $(SRC_DIR)/s21_cat.h
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/s21_common.o: $(COMMON_DIR)/s21_common.c ../common/s21_common.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) $(SRC_DIR)/$(TARGET)

rebuild: clean all

.PHONY: all clean rebuild