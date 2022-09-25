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
	@echo Compiled... Please check ./bbsh.exe

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR) $(OBJS_DIR)
	@$(CC) $(INC_FLAGS) -lm -c $< -o $@

clean:
	@rm -rf ./build
	@rm -f bbsh.exe
	@echo Cleaned Files

# CUSTOM COMMANDS, NOT FOR NORMAL USE
full:
	@rm -rf ./build
	@rm -f bbsh.exe
	@make bbsh
	@echo Prefer using make bbsh directly, as some features may not work here
	@./bbsh.exe
	
	@rm -rf ./build
	@rm -f bbsh.exe
	@echo Cleaned Files

debug: $(OBJS)
	@$(CC) $(INC_FLAGS) -g $(OBJS) -lm -o $(TARGET_EXEC)
	@chmod +x $(TARGET_EXEC)
	@echo Compiled... Running
	@gdb ./$(TARGET_EXEC)
	
	@rm -rf ./build
	@rm -f bbsh.exe
	@echo Cleaned Files

valgrind: $(OBJS)
	@$(CC) $(INC_FLAGS) -g $(OBJS) -lm -o $(TARGET_EXEC)
	@chmod +x $(TARGET_EXEC)
	@echo Compiled... Running
	@valgrind ./$(TARGET_EXEC)
	
	@rm -rf ./build
	@rm -f bbsh.exe
	@echo Cleaned Files
