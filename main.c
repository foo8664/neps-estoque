#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "estoque.txt"
#define MAXNAME 512

#if defined(__WIN32__) || defined(__WIN64__)
# define NEWLINE "\r\n" /* Handling windows bs */
#else
# define NEWLINE "\n"
#endif

enum act {ACT_ERR=-1, ACT_ADD=1, ACT_RM, ACT_LIST, ACT_EXIT};
struct item {
	char *name;
	unsigned am;
};

static void banner(void) __attribute__((nothrow, cold));
static enum act getact(void) __attribute__((nothrow));
static int again(void) __attribute__((nothrow));
static void exit_program(void) __attribute__((nothrow, noreturn));
static void panic(const char *str) __attribute__((nothrow, nonnull, noreturn));
static int add_item(void) __attribute__((nothrow));
static struct item *getitem(FILE *fp) __attribute__((nothrow, nonnull,
	malloc(free, 1)));
static int list_all(void) __attribute__((nothrow));

int main(void)
{
	enum act act;

	banner();
	while (1) {
		switch ((act = getact())) {
		case ACT_ADD:
			if (add_item() == -1)
				panic("add_item()");
			break;
		case ACT_LIST:
			if (list_all() == -1)
				panic("list_all()");
			break;
		default:
			fputs("Opção inválida! Tente novamente.\n", stdout);
			continue;
		}

		if (!again())
			exit_program();
	}
}

static void banner(void)
{
	fputs("================================\n", 	stdout);
	fputs("   Controle de Estoque\n", 		stdout);
	fputs("================================\n", 	stdout);
}

static enum act getact(void)
{
	enum act act;

	fputs("Selecione uma opção:\n", stdout);
	fputs("1. Adicionar Item\n", 	stdout);
	fputs("2. Remover Item\n", 	stdout);
	fputs("3. Listar Estoque\n", 	stdout);
	fputs("4. Sair\n", 		stdout);
	fputs("Opção: ", 		stdout);
	if (scanf("%d", (int *)&act) != 1 || act < 1 || act > 4)
		act = ACT_ERR;
	getchar();
	return act;
}

static int again(void)
{
	char ret;

	while (1) {
		fputs("Deseja realizar outra operação? (s/n): ", stdout);
		scanf("%c", &ret);
		getchar();
		if (ret == 's' || ret == 'S' || ret == 'n' || ret == 'N')
			return ret == 's' || ret == 'S';
	}
}

static void exit_program(void)
{
	fputs("Obrigado por usar o Controle de Estoques! Até a próxima.\n", stdout);
	exit(EXIT_SUCCESS);
	__builtin_unreachable();
}

static int add_item(void)
{
	struct item it;
	FILE *fp;

	if (!(it.name = malloc(MAXNAME * sizeof(*it.name))))
		return -1;
	if (!(fp = fopen(FILENAME, "a"))) {
		free(it.name);
		return -1;
	}

	fputs("Digite o nome do item: ", stdout);
	fgets(it.name, MAXNAME, stdin);
	it.name[strcspn(it.name, "\n")] = '\0';

	fputs("Digite a quantidade: ",	stdout);
	scanf("%u", &it.am);
	getchar();

	fprintf(fp, "%s" NEWLINE "%u" NEWLINE, it.name, it.am);
	fputs("Item adicionado com sucesso!\n", stdout);

	free(it.name);
	fclose(fp);
	return 0;
}

static void panic(const char *str)
{
	fprintf(stderr, "PANIC: %s\n", str);
	exit(EXIT_FAILURE);
	__builtin_unreachable();
}

static struct item *getitem(FILE *fp)
{
	struct item *it;
	char *tmpname;

	if (!(it = malloc(sizeof(*it))))
		return NULL;
	if (!(it->name = malloc(MAXNAME * sizeof(it->name)))) {
		free(it);
		return NULL;
	}

	if (!fgets(it->name, MAXNAME, fp) || fscanf(fp, "%u", &it->am) != 1) {
		free(it->name);
		free(it);
		return NULL;
	}
	it->name[strcspn(it->name, "\n")] = '\0';
	fgetc(fp);

	if (!(tmpname = realloc(it->name, (strlen(it->name) + 1) *
			sizeof(*it->name)))) {
		free(it);
		return NULL;
	}

	it->name = tmpname;
	return it;
}

static int list_all(void)
{
	struct item *it;
	FILE *fp;
	int ch;

	if (!(fp = fopen(FILENAME, "r")))
		return -1;

	ch = fgetc(fp);
	if (feof(fp)) {
		fputs("O estoque esta vazio\n", stdout);
		fclose(fp);
		return 0;
	}
	ungetc(ch, fp);

	while ((it = getitem(fp)) && !feof(fp)) {
		printf("Nome: %s\nQuantidade: %u\n\n", it->name, it->am);
		free(it->name);
		free(it);
	}

	fclose(fp);
	return 0;
}
