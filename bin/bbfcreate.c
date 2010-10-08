#include "ccv.h"

void print_help()
{
	printf("format: bbfcreate posfile posnum negfile negnum\n");
}

int main(int argc, char** argv)
{
	if (argc != 5)
	{
		print_help();
		return -1;
	}
	int i, rt;
	int posnum = atoi(argv[2]);
	FILE* pf = fopen(argv[1], "r");
	ccv_dense_matrix_t** posimg = (ccv_dense_matrix_t**)malloc(sizeof(posimg[0]) * posnum);
	for (i = 0; i < posnum; i++)
	{
		char buf[1024];
		rt = fscanf(pf, "%s", buf);
		ccv_unserialize(buf, &posimg[i], CCV_SERIAL_GRAY | CCV_SERIAL_ANY_FILE);
	}
	fclose(pf);
	int negnum = atoi(argv[4]);
	FILE* bgf = fopen(argv[3], "r");
	int bgnum;
	rt = fscanf(bgf, "%d", &bgnum);
	char** bgfiles = (char**)malloc(sizeof(bgfiles[0]) * bgnum);
	for (i = 0; i < bgnum; i++)
	{
		bgfiles[i] = (char*)malloc(1024);
		rt = fscanf(bgf, "%s", bgfiles[i]);
	}
	fclose(bgf);
	ccv_bbf_param_t params;
	params.pos_crit = 0.995;
	params.neg_crit = 0.50;
	params.balance_k = 1.0;
	params.layer = 24;
	params.feature_number = 25;
	params.optimizer = CCV_BBF_GENETIC_OPT;
	ccv_bbf_classifier_cascade_new(posimg, posnum, bgfiles, bgnum, negnum, ccv_size(32, 32), "data", params);
	for (i = 0; i < bgnum; i++)
		free(bgfiles[i]);
	for (i = 0; i < posnum; i++)
		ccv_matrix_free(&posimg[i]);
	free(posimg);
	free(bgfiles);
	ccv_garbage_collect();
	return 0;
}