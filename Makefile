TARGET_EXEC := bbsh.exe
INC_DIRS := include
BUILD_DIR := build
OBJS_DIR := build/objs
SRC_DIR := src

CC=gcc

INC_FLAGS := -I$(INC_DIRS)

SRCS := $(shell find $(SRC_DIR) -name '*.c' -or -name '*.s')
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJS_DIR)/%.o)

bbsh: $(OBJS) # Won't delete the files, only makes
	@$(CC) $(INC_FLAGS) -g $(OBJS) -lm -o $(TARGET_EXEC)
	@chmod +x $(TARGET_EXEC)
	@echo Compiled... Running
	@./$(TARGET_EXEC)

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR) $(OBJS_DIR)
	@$(CC) $(INC_FLAGS) -lm -c $< -o $@

full: $(OBJS)
	@make bbsh
	
	@rm -rf ./build
	@rm -f bbsh.exe
	@echo Cleaned Files

valgrind:
	@$(CC) $(INC_FLAGS) -g $(OBJS) -lm -o $(TARGET_EXEC)
	@chmod +x $(TARGET_EXEC)
	@echo Compiled... Running
	@valgrind ./$(TARGET_EXEC)
	
	@rm -rf ./build
	@rm -f bbsh.exe
	@echo Cleaned Files

clean:
	@rm -rf ./build
	@rm -f bbsh.exe
	@echo Cleaned Files