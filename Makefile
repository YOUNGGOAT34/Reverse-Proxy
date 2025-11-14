CC :=gcc

FLAGS := -Wall -Wextra
LDFLAGS := -lm
SRC := $(wildcard *.c)
OBJECTS := $(SRC:.c=.o)
BUILD_DIR:=build
OBJECTS:=$(addprefix $(BUILD_DIR)/,$(OBJECTS))
TARGET :=echopulse

all:clean $(TARGET) 
$(TARGET):$(OBJECTS)
			$(CC) -o $@ $^ $(LDFLAGS)
$(BUILD_DIR)/%.o:%.c
						mkdir -p $(BUILD_DIR)
						$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)


