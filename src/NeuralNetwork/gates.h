#include "neuronsUtils.h"
#include "stdlib.h"
#include "learning.h"

ExempleSet *NGetAndExempleSet();

ExempleSet *NGetOrExempleSet();

ExempleSet *NGetXorExempleSet();

NetworkSet* NInitGatesNetworkSet(int gate, int archi, int learning,
	FunctionId input, FunctionId output, FunctionId others, FunctionId bias);
