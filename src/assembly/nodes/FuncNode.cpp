//
//                 __  __            __           
//                / / / /__  __ ____/ /_____ ____ 
//               / /_/ // / / // __  // ___// __ \
//              / __  // /_/ // /_/ // /   / /_/ /
//             /_/ /_/ \__, / \__,_//_/    \____/ 
//                    /____/                      
//
//              The Hydro Programming Language
//
//        © 2020 Shockazoid, Inc. All Rights Reserved.
//

#include "FuncNode.hpp"
#include "TryNode.hpp"

namespace hydro
{

FuncArgNode::FuncArgNode(FuncArg *arg, InstrGroupNode *defaultVal) : BlockNode{}, _arg{arg}, _defaultVal{defaultVal}
{
	if (_defaultVal)
		_defaultVal->returnsValue = true;
}

FuncArgNode::~FuncArgNode() {}

void FuncArgNode::build(Chunk *chunk)
{
	if (_defaultVal)
	{
		// build default value bytes
		_defaultVal->build(chunk);

		// initialize default value
		_arg->defaultVal = _defaultVal->group();
	}
}

FuncNode::FuncNode(VM_Func *func) : BlockNode{}, _func{func}, _args{} {}

FuncNode::~FuncNode() { _args.clear(); }

void FuncNode::appendArg(FuncArgNode *arg)
{
	_args.push_back(arg);
}

void FuncNode::appendTry(TryNode *tryCatch)
{
    _errorHandlers.push_back(tryCatch);
    append(tryCatch); // will emit block
}

void FuncNode::build(Chunk *chunk)
{
    bytearray &bytes = chunk->bytes;
    _func->chunk = chunk;
	_func->nargs = (uint32_t)_args.size();
	_func->nlocals = (uint32_t)_slots.size();
	_func->args = new FuncArg *[_func->nargs];
	_func->locals = new VM_Object *[_func->nlocals];
    
	uint32_t i = 0;

	// emit arguments and populate arguments list
	for (FuncArgNode *arg : _args)
	{
		arg->build(chunk);
		_func->args[i++] = arg->arg();
	}

	i = 0;

	// populate locals list
	for (VM_Object *loc : _slots)
		_func->locals[i++] = loc;

	// emit function body
	_func->startAddr = bytes.position();
	BlockNode::build(chunk); // build children
	_func->endAddr = bytes.position();
    
    uint32_t nhandlers = 0;
    
    // count error handlers
    for(TryNode *tryCatch : _errorHandlers)
        nhandlers += (uint32_t) tryCatch->catches().size();
    
    _func->nhandlers = nhandlers;
    _func->errorHandlers = new ErrorHandler*[_func->nhandlers];
    
    i = 0;
    
    for(uint32_t j = 0; j < _errorHandlers.size(); j++)
    {
        TryNode *tryCatch = _errorHandlers[j];
        auto catches = tryCatch->catches();
        uint32_t n = (uint32_t)catches.size();
        
        for(uint32_t k = 0; k < n; k++)
        {
            CatchNode *clause = catches[k];
            _func->errorHandlers[i++] = clause->errorHandler();
        }
    }
}

} // namespace hydro
