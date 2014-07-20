#pragma once

#include <iostream>

# undef DEBUG_MESSAGE
# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	assert(0 && std::string(std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "].").c_str()); return return_type; }


namespace gl
{
	void setClearValue(void **);
	void setColorMask(void **);
	void setDepthMask(void **);
	void setStencilMask(void **);
	void clear(void **);
	void setScissor(void **);
	void setStencilFunctionFrontFace(void **);
	void setStencilOperationFrontFace(void **);
	void setStencilFunctionBackFace(void **);
	void setStencilOperationBackFace(void **);
	void setStencilFunction(void **);
	void setStencilOperation(void **);
	void setBlend(void **);
	void setBlendEquationSeparate(void **);
	void setBlendEquation(void **);
	void setBlendFuncSeparate(void **);
	void setBlendFunc(void **);
	void setBlendConstant(void **);
	void setTest(void **);
	void setUniformMat4(void **);
	void setUniformMat3(void **);
	void setUniformUint(void **);
	void setBlockPointerUBO(void **);
	void setUniformFloat(void **);
	void setUniformVec4(void **);

	struct Task
	{
		int nbrParams;
		void **params;
		int *sizeParams;
		void(*func)(void **);
		Task() : nbrParams(0), params(NULL), sizeParams(NULL), func(NULL){}
	};

	template <typename TYPE>
	void setTaskAllocation(Task &task, TYPE element)
	{
		int index = task.nbrParams;
		task.nbrParams = task.nbrParams + 1;
		task.params = new void *[task.nbrParams];
		task.sizeParams = new int *[task.nbrParams];
		task.params[index] = new TYPE;
		memcpy(task.params[index], &element, sizeof(TYPE));
		task.sizeParams[index] = sizeof(TYPE);
	}

	template <typename TYPE, typename... TYPES>
	void setTaskAllocation(Task &task, TYPE element, TYPES... elements)
	{
		int index = task.nbrParams;
		task.nbrParams = task.nbrParams + 1;
		setTaskAllocation(task, elements...);
		task.params[index] = new TYPE;
		memcpy(task.params[index], &element, sizeof(TYPE));
		task.sizeParams[index] = sizeof(TYPE);
	}
}