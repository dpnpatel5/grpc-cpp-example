#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <grpcpp/grpcpp.h>
#include "calculator_api.h"
#include "calculator.grpc.pb.h"

#define MAX_CMD_NUM 5

#define SHOW_HELP(ind)  do { \
        printf("%s", help_info[ind][0]); \
        printf("%s", help_info[ind][1]); \
        return -1; \
} while(0)


typedef struct {
	const char * cmd_name;
	int (* f)(int argc, char ** argv, int ind);
} cmd_entry;


const char * help_info[MAX_CMD_NUM][2] = {
	{"sum <a> <b>\n\r","Provide 2 nums to sum)\n\r"},
	{"sub <a> <b>\n\r","Provide 2 nums to sub)\n\r"},
	{"mul <a> <b>\n\r","Provide 2 nums to mul)\n\r"},
	{"div <a> <b>\n\r","Provide 2 nums to div)\n\r"},
};
int sum_stub(int argc, char ** argv, int ind)
{

        int ret;
        int a , b;

        if (argc < 4)
                SHOW_HELP(ind);

        if (sscanf(argv[2], "%d", &a) < 1)
                SHOW_HELP(ind);

        if (sscanf(argv[3], "%d", &b) < 1)
                SHOW_HELP(ind);

        ret = add_api(a , b);
	printf("Result is: %d\n", ret);
	return 0;
}
int sub_stub(int argc, char ** argv, int ind)
{

        int ret;
        int a , b;

        if (argc < 4)
                SHOW_HELP(ind);

        if (sscanf(argv[2], "%d", &a) < 1)
                SHOW_HELP(ind);

        if (sscanf(argv[3], "%d", &b) < 1)
                SHOW_HELP(ind);

        ret = sub_api(a , b);
	printf("Result is: %d\n", ret);
	return 0;
}
int mul_stub(int argc, char ** argv, int ind)
{

        int ret;
        int a , b;

        if (argc < 4)
                SHOW_HELP(ind);

        if (sscanf(argv[2], "%d", &a) < 1)
                SHOW_HELP(ind);

        if (sscanf(argv[3], "%d", &b) < 1)
                SHOW_HELP(ind);

        ret = mul_api(a , b);
	printf("Result is: %d\n", ret);
	return 0;
}
int div_stub(int argc, char ** argv, int ind)
{

        int ret;
        int a , b;

        if (argc < 4)
                SHOW_HELP(ind);

        if (sscanf(argv[2], "%d", &a) < 1)
                SHOW_HELP(ind);

        if (sscanf(argv[3], "%d", &b) < 1)
                SHOW_HELP(ind);

        ret = div_api(a , b);
	printf("Result is: %d\n", ret);
	return 0;
}

cmd_entry cmd_table[MAX_CMD_NUM] = {
	{"sum", sum_stub},
	{"sub", sub_stub},
	{"mul", mul_stub},
	{"div", div_stub},
};

void print_help()
{
	int i;

	for (i = 0; i < MAX_CMD_NUM; i++) {
		if (NULL == help_info[i][0])
			break;
		printf("%s", help_info[i][0]);
	}
}

void print_version(void)
{
	printf("Calculator Built On %s %s\n\r", __DATE__, __TIME__);
}

int parse_cmd(int argc, char ** argv)
{
	int i, ret;
	int found;

	ret = -EINVAL;
	found = 0;

	for (i = 0; i < MAX_CMD_NUM; i++) {
		if (!cmd_table[i].cmd_name)
			continue;

		if (!strcmp(cmd_table[i].cmd_name, argv[1])) {
			found = 1;
			ret = (*cmd_table[i].f)(argc, argv, i);
			break;
		}
	}

	if (!found) {
		printf("Cmd \"%s\" not found\n\r", argv[1]);
		print_help();
	}

	return ret;
}

int parse_opt(int argc, char ** argv, char ** target)
{
	int opt, i;
	char* cmd;

	cmd = NULL;

	opterr = 0;	/* Don't let getopt() print out error message */

	while ((opt = getopt(argc, argv, "+h:s:")) != -1) {
		switch(opt) {
			case 'h':
				cmd = optarg;
				for (i = 0; i < MAX_CMD_NUM; i++) {
					if (!cmd_table[i].cmd_name)
						continue;

					if (!strncmp(cmd_table[i].cmd_name, 
								cmd, strlen(cmd_table[i].cmd_name))) {
						printf("%s", help_info[i][0]);
						printf("%s", help_info[i][1]);

						break;
					}
				}	

				return -1;
				break;

			case 's':
				*target = optarg;
				return 1;

				break;

			case '?':
				if (optopt == 'h' || optopt == 's') {
					print_help();
					return -1;
				}

				/* Let negative values(e.g. -40) to go through */

				break;
		}
	}

	return 0;
}
int main(int argc, char** argv) 
{
	int i,ret;
	char * target_str = "localhost:50051";

	if (argc < 2) {
		print_help();
		return -1;
	}

	if((0 == strcmp(argv[1],"version")) || (0 == strcmp(argv[1],"--v"))) {
		print_version();
		return -1;
	}

	ret = parse_opt(argc, argv, &target_str);
	if (ret < 0)
		return -1;

	if (optind >= argc) {
		return -1;
	}

	if (ret > 0) {
		argc = argc - optind + 1;
		for (i = 1; i < argc; i++) {
			argv[i] = argv[optind + i - 1];
		}

	}
	if (connect_server(target_str) < 0 ) 
	{
		printf("Failed to connect server!\n\r");
		return -1;
	}
	ret = parse_cmd(argc, argv);
	return ret;
}
