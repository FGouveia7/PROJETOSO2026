# ============================================================================
# TODO (Francisco / servidor): quando o main() do servidor estiver feito,
# adicionar o ficheiro com o main em SERVER_SRC.
# Sem um main(), "make" compila os .o mas o linker falha com:
#       undefined reference to `main'
# ============================================================================

#O codigo foi feito com auxilio de: https://www.youtube.com/watch?v=HmbByRhh3Sk 

CC = gcc 
CFLAGS = -Wall -Wextra -g -pthread -Iserver -Ishared -MMD -MP #-Wall -Wextra para avisos, -g debug, -pthread threads, -I caminhos dos .h, -MMD -MP dependencias dos .h
LDFLAGS = -pthread #flags do linker (threads)


#wildcard com todos os .c das pastas share e server
SHARED_SRC = $(wildcard shared/*.c)
SERVER_SRC = $(wildcard server/*.c) \
             $(wildcard server/leitura/*.c) \
             $(wildcard server/validacao/*.c) \
             $(wildcard server/escrita/*.c) \
             $(SHARED_SRC)
SERVER_OBJ = $(SERVER_SRC:.c=.o)

#nao e um ficheiro mas sim um comando que vai ser executado quando fizer make all
all: servidor 

#adicionar todos os .o aqui 
servidor: $(SERVER_OBJ)
	$(CC) $(LDFLAGS) $(SERVER_OBJ) -o servidor

#pattern role 
%.o:    %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SERVER_OBJ) $(SERVER_OBJ:.o=.d) servidor

#dependencias do .h
-include $(SERVER_OBJ:.o=.d)

.PHONY: all clean