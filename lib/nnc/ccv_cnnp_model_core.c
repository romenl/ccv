#include "ccv_nnc.h"
#include "ccv_nnc_easy.h"
#include "ccv_nnc_internal.h"
#include "ccv_internal.h"
#include "_ccv_cnnp_model.h"

#pragma mark - Core Layers

static void _ccv_cnnp_add_build(ccv_cnnp_model_t* const self, ccv_nnc_symbolic_graph_t* const graph, const ccv_nnc_tensor_symbol_t* const inputs, const int input_size, ccv_nnc_tensor_symbol_t* const outputs, const int output_size)
{
	assert(output_size == 1);
	outputs[0] = ccv_nnc_tensor_symbol_new(graph, ccv_nnc_tensor_symbol_params(graph, inputs[0]), 0);
	ccv_nnc_graph_exec_symbol_new(graph, CMD_EWSUM_FORWARD(), inputs, input_size, outputs, output_size, 0);
}

static const ccv_cnnp_model_vtab_t ccv_cnnp_add_isa = {
	.build = _ccv_cnnp_add_build,
};

typedef struct {
	ccv_cnnp_model_t super;
	ccv_nnc_tensor_symbol_t output;
} ccv_cnnp_model_add_t;

ccv_cnnp_model_t* ccv_cnnp_add(void)
{
	ccv_cnnp_model_add_t* const model_add = (ccv_cnnp_model_add_t*)cccalloc(1, sizeof(ccv_cnnp_model_add_t));
	model_add->super.isa = &ccv_cnnp_add_isa;
	model_add->super.input_size = 1;
	model_add->super.outputs = &model_add->output;
	model_add->super.output_size = 1;
	return (ccv_cnnp_model_t*)model_add;
}

static void _ccv_cnnp_concat_build(ccv_cnnp_model_t* const self, ccv_nnc_symbolic_graph_t* const graph, const ccv_nnc_tensor_symbol_t* const inputs, const int input_size, ccv_nnc_tensor_symbol_t* const outputs, const int output_size)
{
	assert(output_size == 1);
}

static const ccv_cnnp_model_vtab_t ccv_cnnp_concat_isa = {
	.build = _ccv_cnnp_concat_build,
};

typedef struct {
	ccv_cnnp_model_t super;
	ccv_nnc_tensor_symbol_t output;
} ccv_cnnp_model_concat_t;

ccv_cnnp_model_t* ccv_cnnp_concat(void)
{
	ccv_cnnp_model_concat_t* const model_concat = (ccv_cnnp_model_concat_t*)cccalloc(1, sizeof(ccv_cnnp_model_concat_t));
	model_concat->super.isa = &ccv_cnnp_concat_isa;
	model_concat->super.input_size = 1;
	model_concat->super.outputs = &model_concat->output;
	model_concat->super.output_size = 1;
	return (ccv_cnnp_model_t*)model_concat;
}

typedef struct {
	ccv_cnnp_model_t super;
	ccv_nnc_tensor_symbol_t output;
	int dim[CCV_NNC_MAX_DIM_ALLOC];
} ccv_cnnp_model_reshape_t;

static void _ccv_cnnp_reshape_build(ccv_cnnp_model_t* const super, ccv_nnc_symbolic_graph_t* const graph, const ccv_nnc_tensor_symbol_t* const inputs, const int input_size, ccv_nnc_tensor_symbol_t* const outputs, const int output_size)
{
	assert(input_size == 1);
	assert(output_size == 1);
	ccv_cnnp_model_reshape_t* const self = (ccv_cnnp_model_reshape_t*)super;
	ccv_nnc_tensor_param_t params = ccv_nnc_tensor_symbol_params(graph, inputs[0]);
	memcpy(params.dim, self->dim, sizeof(params.dim));
	outputs[0] = ccv_nnc_tensor_symbol_alias_new(graph, inputs[0], DIM_ALLOC(), self->dim, params, 0);
}

static const ccv_cnnp_model_vtab_t ccv_cnnp_reshape_isa = {
	.build = _ccv_cnnp_reshape_build,
};

ccv_cnnp_model_t* ccv_cnnp_reshape(const int dim[CCV_NNC_MAX_DIM_ALLOC])
{
	ccv_cnnp_model_reshape_t* const model_reshape = (ccv_cnnp_model_reshape_t*)cccalloc(1, sizeof(ccv_cnnp_model_reshape_t));
	model_reshape->super.isa = &ccv_cnnp_reshape_isa;
	model_reshape->super.input_size = 1;
	model_reshape->super.outputs = &model_reshape->output;
	model_reshape->super.output_size = 1;
	memcpy(model_reshape->dim, dim, sizeof(model_reshape->dim));
	return (ccv_cnnp_model_t*)model_reshape;
}

typedef struct {
	ccv_cnnp_model_t super;
	ccv_nnc_tensor_symbol_t output;
} ccv_cnnp_model_flatten_t;

static void _ccv_cnnp_flatten_build(ccv_cnnp_model_t* const super, ccv_nnc_symbolic_graph_t* const graph, const ccv_nnc_tensor_symbol_t* const inputs, const int input_size, ccv_nnc_tensor_symbol_t* const outputs, const int output_size)
{
	assert(input_size == 1);
	assert(output_size == 1);
	const ccv_nnc_tensor_param_t params = ccv_nnc_tensor_symbol_params(graph, inputs[0]);
	ccv_nnc_tensor_param_t output_params = params;
	memset(output_params.dim, 0, sizeof(output_params.dim));
	output_params.dim[0] = ccv_nnc_tensor_get_n(params);
	output_params.dim[1] = ccv_nnc_tensor_count(params) / output_params.dim[0];
	outputs[0] = ccv_nnc_tensor_symbol_alias_new(graph, inputs[0], DIM_ALLOC(), output_params.dim, output_params, 0);
}

static const ccv_cnnp_model_vtab_t ccv_cnnp_flatten_isa = {
	.build = _ccv_cnnp_flatten_build,
};

ccv_cnnp_model_t* ccv_cnnp_flatten(void)
{
	ccv_cnnp_model_flatten_t* const model_flatten = (ccv_cnnp_model_flatten_t*)cccalloc(1, sizeof(ccv_cnnp_model_flatten_t));
	model_flatten->super.isa = &ccv_cnnp_flatten_isa;
	model_flatten->super.input_size = 1;
	model_flatten->super.outputs = &model_flatten->output;
	model_flatten->super.output_size = 1;
	return (ccv_cnnp_model_t*)model_flatten;
}

#pragma mark - Convolution Layer

typedef struct {
	ccv_cnnp_model_t super;
	ccv_nnc_tensor_symbol_t output;
	ccv_nnc_tensor_symbol_t weights;
	ccv_nnc_tensor_symbol_t bias;
	int groups;
	int filters;
	int kdim[CCV_NNC_MAX_DIM_ALLOC];
	ccv_cnnp_param_t params;
} ccv_cnnp_model_convolution_t;

static void _ccv_cnnp_convolution_build(ccv_cnnp_model_t* const super, ccv_nnc_symbolic_graph_t* const graph, const ccv_nnc_tensor_symbol_t* const inputs, const int input_size, ccv_nnc_tensor_symbol_t* const outputs, const int output_size)
{
	assert(input_size == 1);
	assert(output_size == 1);
	ccv_cnnp_model_convolution_t* const self = (ccv_cnnp_model_convolution_t*)super;
	const ccv_nnc_tensor_param_t params = ccv_nnc_tensor_symbol_params(graph, inputs[0]);
	int i;
	const int nd = CCV_NNC_MAX_DIM + 2;
	ccv_nnc_tensor_param_t weights_params = params;
	ccv_nnc_tensor_set_n(&weights_params, self->filters);
	assert(ccv_nnc_tensor_get_c(params) % self->groups == 0);
	ccv_nnc_tensor_set_c(&weights_params, nd, ccv_nnc_tensor_get_c(params) / self->groups);
	const int hw = ccv_nnc_tensor_hw(weights_params, nd);
	assert(hw >= 0);
	for (i = 0; i < CCV_NNC_MAX_DIM; i++)
		weights_params.dim[i + hw] = self->kdim[i];
	if (!self->weights.graph)
		self->weights = ccv_nnc_tensor_symbol_new(graph, weights_params, 0);
	assert(self->weights.graph == graph);
	ccv_nnc_tensor_param_t bias_params = params;
	memset(bias_params.dim, 0, sizeof(bias_params.dim));
	bias_params.dim[0] = self->filters;
	if (!self->bias.graph)
		self->bias = ccv_nnc_tensor_symbol_new(graph, bias_params, 0);
	ccv_nnc_cmd_t cmd = CMD_CONVOLUTION_FORWARD(self->groups, self->filters);
	for (i = 0; i < CCV_NNC_MAX_DIM; i++)
		cmd.info.size.dim[i] = self->kdim[i];
	ccv_nnc_tensor_param_t output_params;
	ccv_nnc_hint_tensor_auto(cmd, (ccv_nnc_tensor_param_t []){
			params,
			weights_params,
			bias_params,
		}, 3, self->params.hint, &output_params, 1);
	const ccv_nnc_tensor_symbol_t convolution_output = ccv_nnc_tensor_symbol_new(graph, output_params, 0);
	const ccv_nnc_graph_exec_symbol_t convolution = ccv_nnc_graph_exec_symbol_new(graph, cmd, TENSOR_SYMBOL_LIST(inputs[0], self->weights, self->bias), TENSOR_SYMBOL_LIST(convolution_output), 0);
	ccv_nnc_graph_exec_symbol_set_hint(graph, convolution, self->params.hint);
	if (self->params.activation == CCV_CNNP_ACTIVATION_RELU)
	{
		const ccv_nnc_tensor_symbol_t relu_output = ccv_nnc_tensor_symbol_new(graph, output_params, 0);
		ccv_nnc_graph_exec_symbol_new(graph, CMD_RELU_FORWARD(), TENSOR_SYMBOL_LIST(convolution_output), TENSOR_SYMBOL_LIST(relu_output), 0);
		outputs[0] = relu_output;
	} else if (self->params.activation == CCV_CNNP_ACTIVATION_SOFTMAX) {
		const ccv_nnc_tensor_symbol_t softmax_output = ccv_nnc_tensor_symbol_new(graph, output_params, 0);
		ccv_nnc_graph_exec_symbol_new(graph, CMD_SOFTMAX_FORWARD(), TENSOR_SYMBOL_LIST(convolution_output), TENSOR_SYMBOL_LIST(softmax_output), 0);
		outputs[0] = softmax_output;
	} else
		outputs[0] = convolution_output;
}

static const ccv_cnnp_model_vtab_t ccv_cnnp_convolution_isa = {
	.build = _ccv_cnnp_convolution_build,
};

ccv_cnnp_model_t* ccv_cnnp_convolution(const int groups, const int filters, const int kdim[CCV_NNC_MAX_DIM_ALLOC], const ccv_cnnp_param_t params)
{
	ccv_cnnp_model_convolution_t* const model_convolution = (ccv_cnnp_model_convolution_t*)cccalloc(1, sizeof(ccv_cnnp_model_convolution_t));
	model_convolution->super.isa = &ccv_cnnp_convolution_isa;
	model_convolution->super.input_size = 1;
	model_convolution->super.outputs = &model_convolution->output;
	model_convolution->super.output_size = 1;
	model_convolution->weights.d = CCV_NNC_NO_TENSOR_SYMBOL;
	model_convolution->weights.graph = 0;
	model_convolution->bias.d = CCV_NNC_NO_TENSOR_SYMBOL;
	model_convolution->bias.graph = 0;
	model_convolution->groups = groups;
	model_convolution->filters = filters;
	memcpy(model_convolution->kdim, kdim, sizeof(model_convolution->kdim));
	model_convolution->params = params;
	return (ccv_cnnp_model_t*)model_convolution;
}

#pragma mark - Dense Layer

typedef struct {
	ccv_cnnp_model_t super;
	ccv_nnc_tensor_symbol_t output;
	ccv_nnc_tensor_symbol_t weights;
	ccv_nnc_tensor_symbol_t bias;
	int count;
	ccv_cnnp_param_t params;
} ccv_cnnp_model_dense_t;

static void _ccv_cnnp_dense_build(ccv_cnnp_model_t* const super, ccv_nnc_symbolic_graph_t* const graph, const ccv_nnc_tensor_symbol_t* const inputs, const int input_size, ccv_nnc_tensor_symbol_t* const outputs, const int output_size)
{
	assert(input_size == 1);
	assert(output_size == 1);
	ccv_cnnp_model_dense_t* const self = (ccv_cnnp_model_dense_t*)super;
	const ccv_nnc_tensor_param_t params = ccv_nnc_tensor_symbol_params(graph, inputs[0]);
	ccv_nnc_tensor_param_t weights_params = params;
	memset(weights_params.dim, 0, sizeof(weights_params.dim));
	weights_params.dim[0] = self->count;
	weights_params.dim[1] = ccv_nnc_tensor_get_c(params);
	if (!self->weights.graph)
		self->weights = ccv_nnc_tensor_symbol_new(graph, weights_params, 0);
	assert(self->weights.graph == graph);
	ccv_nnc_tensor_param_t bias_params = params;
	memset(bias_params.dim, 0, sizeof(bias_params.dim));
	bias_params.dim[0] = self->count;
	if (!self->bias.graph)
		self->bias = ccv_nnc_tensor_symbol_new(graph, bias_params, 0);
	const ccv_nnc_cmd_t cmd = CMD_GEMM_FORWARD(self->count);
	ccv_nnc_tensor_param_t output_params;
	ccv_nnc_hint_tensor_auto(cmd, (ccv_nnc_tensor_param_t []){
			params,
			weights_params,
			bias_params,
		}, 3, self->params.hint, &output_params, 1);
	const ccv_nnc_tensor_symbol_t dense_output = ccv_nnc_tensor_symbol_new(graph, output_params, 0);
	ccv_nnc_graph_exec_symbol_new(graph, cmd, TENSOR_SYMBOL_LIST(inputs[0], self->weights, self->bias), TENSOR_SYMBOL_LIST(dense_output), 0);
	if (self->params.activation == CCV_CNNP_ACTIVATION_RELU)
	{
		const ccv_nnc_tensor_symbol_t relu_output = ccv_nnc_tensor_symbol_new(graph, output_params, 0);
		ccv_nnc_graph_exec_symbol_new(graph, CMD_RELU_FORWARD(), TENSOR_SYMBOL_LIST(dense_output), TENSOR_SYMBOL_LIST(relu_output), 0);
		outputs[0] = relu_output;
	} else if (self->params.activation == CCV_CNNP_ACTIVATION_SOFTMAX) {
		const ccv_nnc_tensor_symbol_t softmax_output = ccv_nnc_tensor_symbol_new(graph, output_params, 0);
		ccv_nnc_graph_exec_symbol_new(graph, CMD_SOFTMAX_FORWARD(), TENSOR_SYMBOL_LIST(dense_output), TENSOR_SYMBOL_LIST(softmax_output), 0);
		outputs[0] = softmax_output;
	} else
		outputs[0] = dense_output;
}

static const ccv_cnnp_model_vtab_t ccv_cnnp_dense_isa = {
	.build = _ccv_cnnp_dense_build,
};

ccv_cnnp_model_t* ccv_cnnp_dense(const int count, const ccv_cnnp_param_t params)
{
	ccv_cnnp_model_dense_t* const model_dense = (ccv_cnnp_model_dense_t*)cccalloc(1, sizeof(ccv_cnnp_model_dense_t));
	model_dense->super.isa = &ccv_cnnp_dense_isa;
	model_dense->super.input_size = 1;
	model_dense->super.outputs = &model_dense->output;
	model_dense->super.output_size = 1;
	model_dense->weights.d = CCV_NNC_NO_TENSOR_SYMBOL;
	model_dense->weights.graph = 0;
	model_dense->bias.d = CCV_NNC_NO_TENSOR_SYMBOL;
	model_dense->bias.graph = 0;
	model_dense->count = count;
	model_dense->params = params;
	return (ccv_cnnp_model_t*)model_dense;
}

#pragma mark - Pool Layers

typedef struct {
	ccv_cnnp_model_t super;
	ccv_nnc_tensor_symbol_t output;
	int kdim[CCV_NNC_MAX_DIM_ALLOC];
	ccv_cnnp_param_t params;
} ccv_cnnp_model_pool_t;

static void _ccv_cnnp_max_pool_build(ccv_cnnp_model_t* const super, ccv_nnc_symbolic_graph_t* const graph, const ccv_nnc_tensor_symbol_t* const inputs, const int input_size, ccv_nnc_tensor_symbol_t* const outputs, const int output_size)
{
	assert(input_size == 1);
	assert(output_size == 1);
	ccv_cnnp_model_pool_t* const self = (ccv_cnnp_model_pool_t*)super;
	const ccv_nnc_tensor_param_t params = ccv_nnc_tensor_symbol_params(graph, inputs[0]);
	const ccv_nnc_cmd_t cmd = CMD_MAX_POOL_FORWARD(self->kdim[0], self->kdim[1]);
	ccv_nnc_tensor_param_t output_params;
	ccv_nnc_hint_tensor_auto(cmd, &params, 1, self->params.hint, &output_params, 1);
	const ccv_nnc_tensor_symbol_t pool_output = ccv_nnc_tensor_symbol_new(graph, output_params, 0);
	ccv_nnc_graph_exec_symbol_new(graph, cmd, TENSOR_SYMBOL_LIST(inputs[0]), TENSOR_SYMBOL_LIST(pool_output), 0);
	outputs[0] = pool_output;
}

static const ccv_cnnp_model_vtab_t ccv_cnnp_max_pool_isa = {
	.build = _ccv_cnnp_max_pool_build,
};

ccv_cnnp_model_t* ccv_cnnp_max_pool(const int kdim[CCV_NNC_MAX_DIM_ALLOC], const ccv_cnnp_param_t params)
{
	ccv_cnnp_model_pool_t* const model_pool = (ccv_cnnp_model_pool_t*)cccalloc(1, sizeof(ccv_cnnp_model_pool_t));
	model_pool->super.isa = &ccv_cnnp_max_pool_isa;
	model_pool->super.input_size = 1;
	model_pool->super.outputs = &model_pool->output;
	model_pool->super.output_size = 1;
	memcpy(model_pool->kdim, kdim, sizeof(model_pool->kdim));
	model_pool->params = params;
	return (ccv_cnnp_model_t*)model_pool;
}

static void _ccv_cnnp_average_pool_build(ccv_cnnp_model_t* const super, ccv_nnc_symbolic_graph_t* const graph, const ccv_nnc_tensor_symbol_t* const inputs, const int input_size, ccv_nnc_tensor_symbol_t* const outputs, const int output_size)
{
	assert(input_size == 1);
	assert(output_size == 1);
	ccv_cnnp_model_pool_t* const self = (ccv_cnnp_model_pool_t*)super;
	const ccv_nnc_tensor_param_t params = ccv_nnc_tensor_symbol_params(graph, inputs[0]);
	const ccv_nnc_cmd_t cmd = CMD_AVERAGE_POOL_FORWARD(self->kdim[0], self->kdim[1]);
	ccv_nnc_tensor_param_t output_params;
	ccv_nnc_hint_tensor_auto(cmd, &params, 1, self->params.hint, &output_params, 1);
	const ccv_nnc_tensor_symbol_t pool_output = ccv_nnc_tensor_symbol_new(graph, output_params, 0);
	ccv_nnc_graph_exec_symbol_new(graph, cmd, TENSOR_SYMBOL_LIST(inputs[0]), TENSOR_SYMBOL_LIST(pool_output), 0);
	outputs[0] = pool_output;
}

static const ccv_cnnp_model_vtab_t ccv_cnnp_average_pool_isa = {
	.build = _ccv_cnnp_average_pool_build,
};

ccv_cnnp_model_t* ccv_cnnp_average_pool(const int kdim[CCV_NNC_MAX_DIM_ALLOC], const ccv_cnnp_param_t params)
{
	ccv_cnnp_model_pool_t* const model_pool = (ccv_cnnp_model_pool_t*)cccalloc(1, sizeof(ccv_cnnp_model_pool_t));
	model_pool->super.isa = &ccv_cnnp_average_pool_isa;
	model_pool->super.input_size = 1;
	model_pool->super.outputs = &model_pool->output;
	model_pool->super.output_size = 1;
	memcpy(model_pool->kdim, kdim, sizeof(model_pool->kdim));
	model_pool->params = params;
	return (ccv_cnnp_model_t*)model_pool;
}
