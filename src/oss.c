#include <hurd.h>

static const struct argp argp = {
	.doc = "Translator for OSS"
};

int main(int argc, char *argv[]) {
	mach_port_t bootstrap;

	argp_parse(&argp, argc, argv, 0, NULL, NULL);

	task_get_bootstrap_port(mach_task_self(), &bootstrap);
	if (bootstrap == MACH_PORT_NULL) {
		error(1, 0, "must be started as translator")
	}
}
