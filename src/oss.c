#define _GNU_SOURCE 1

#include <argp.h>
#include <error.h>
#include <fcntl.h>
#include <hurd.h>
#include <hurd/trivfs.h>
#include <stdio.h>
#include <sys/mman.h>

/* peropen data */
struct peropen_data
{
};

/* trivfs hooks */
int trivfs_fstype = FSTYPE_MISC;
int trivfs_fsid = 0;

int trivfs_allow_open = O_WRITE;

int trivfs_support_read = 0;
int trivfs_support_write = 1;
int trivfs_support_exec = 0;

void trivfs_modify_stat(struct trivfs_protid *cred, io_statbuf_t *stbuf)
{
	stbuf->st_mode &= ~S_IFMT;
	stbuf->st_mode |= S_IFCHR;
	stbuf->st_size = 0;
}

error_t trivfs_goaway(struct trivfs_control *cntl, int flags)
{
	exit(0);
}

/* open trivfs */
error_t open_hook(struct trivfs_peropen *peropen)
{
	struct peropen_data *op = malloc(sizeof(struct peropen_data));
	if (op == NULL) {
		return errno;
	}

	peropen->hook = op;

	return 0;
}

error_t (*trivfs_peropen_create_hook) (struct trivfs_peropen *perop) = open_hook;

/* close trivfs */
void close_hook(struct trivfs_peropen *peropen)
{
	free(peropen->hook);
}

void (*trivfs_peropen_destroy_hook) (struct trivfs_peropen *perop) = close_hook;

/* read from trivfs */
error_t trivfs_S_io_read (struct trivfs_protid *cred,
		mach_port_t reply, mach_msg_type_name_t reply_type,
		vm_address_t *data, mach_msg_type_number_t *data_len,
		off_t offs, mach_msg_type_number_t amount)
{
	if (!cred) {
		return EOPNOTSUPP;
	} else if (! (cred->po->openmodes & O_READ)) {
		return EBADF;
	}

	if (amount > 0) {
		int i;

		if (*data_len < amount) {
			*data = (vm_address_t) mmap (0, amount, PROT_READ|PROT_WRITE,
					MAP_ANON, 0, 0);
		}

		for (i = 0; i < amount; i++) {
			((char *) *data)[i] = 97;
		}
	}

	*data_len = amount;
	return 0;
}

/* write to trivfs */
error_t trivfs_S_io_write (struct trivfs_protid *cred,
		mach_port_t reply, mach_msg_type_name_t reply_type,
		vm_address_t data, mach_msg_type_number_t data_len,
		off_t offs, mach_msg_type_number_t *amount)
{
	if (!cred)
		return EOPNOTSUPP;
	else if (! (cred->po->openmodes & O_WRITE))
		return EBADF;

	*amount = data_len;
	return 0;
}

/* process arguments */
static const struct argp argp = {
	.doc = "Translator for OSS"
};

/* main */
int main(int argc, char *argv[])
{
	error_t err;
	mach_port_t bootstrap;
	struct trivfs_control *fsys;

	err = argp_parse(&argp, argc, argv, 0, NULL, NULL);
	if (err) {
		error(1, err, "argp_parse");
	}

	task_get_bootstrap_port(mach_task_self(), &bootstrap);
	if (bootstrap == MACH_PORT_NULL) {
		error(1, 0, "must be started as translator");
	}

	/* reply to our parent */
	err = trivfs_startup(bootstrap, 0, NULL, NULL, NULL, NULL, &fsys);
	if (err) {
		error(3, err, "trivfs_startup");
	}

	/* launch */
	ports_manage_port_operations_one_thread(fsys->pi.bucket, trivfs_demuxer, 0);

	return 0;
}
