#pragma once

# undef DEBUG_MESSAGE
# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	assert(0 && std::string(std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "].").c_str()); return return_type; }


#define GEN_DEC_RENDER_PUSH_TASK(name) \
	ShadingManager &pushClearTask##name##(Key<##name##> const &key, bool color = true, bool depth = true, bool stencil = false); \
	ShadingManager &pushSetClearValueTask##name##(Key<##name##> const &key, glm::vec4 const &color, float depth = 1.0f, uint8_t stencil = 0); \
	ShadingManager &pushSetColorMaskTask##name##(Key<##name##> const &key, glm::bvec4 const &color, GLuint index = 0); \
	ShadingManager &pushSetDepthMaskTask##name##(Key<##name##> const &key, bool depth); \
	ShadingManager &pushSetStencilMaskTask##name##(Key<##name##> const &key, uint8_t front, uint8_t back); \
	ShadingManager &pushSetTestTask##name##(Key<##name##> const &key, bool scissor, bool stencil, bool depth); \
	ShadingManager &pushSetScissorTask##name##(Key<##name##> const &key, glm::ivec4 const &area); \
	ShadingManager &pushSetStencilFunctionFrontFaceTask##name##(Key<##name##> const &key, GLenum func, int ref, uint8_t mask = 0xff); \
	ShadingManager &pushSetStencilOperationFrontFaceTask##name##(Key<##name##> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass); \
	ShadingManager &pushSetStencilFunctionBackFaceTask##name##(Key<##name##> const &key, GLenum func, int ref, uint8_t mask = 0xff); \
	ShadingManager &pushSetStencilOperationBackFaceTask##name##(Key<##name##> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass); \
	ShadingManager &pushSetStencilFunctionTask##name##(Key<##name##> const &key, GLenum func, int ref, uint8_t mask = 0xff); \
	ShadingManager &pushSetStencilOperationTask##name##(Key<##name##> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass); \
	ShadingManager &pushSetBlendEquationTask##name##(Key<##name##> const &key, GLenum mode); \
	ShadingManager &pushSetBlendEquationTask##name##(Key<##name##> const &key, GLenum colorMode, GLenum alphaMode); \
	ShadingManager &pushSetBlendFuncTask##name##(Key<##name##> const &key, GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha); \
	ShadingManager &pushSetBlendFuncTask##name##(Key<##name##> const &key, GLenum src, GLenum dest); \
	ShadingManager &pushSetBlendConstantTask##name##(Key<##name##> const &key, glm::vec4 const &blendPass); \
	ShadingManager &popTask##name##(Key<##name##> const &key);

#define GEN_DEF_RENDER_PUSH_TASK(name) 																																								\
	ShadingManager &ShadingManager::pushClearTask##name##(Key<##name##> const &key, bool color, bool depth, bool stencil)																			\
	{																																																\
		##name## *renderPass;																																											\
		if ((renderPass = get##name##(key, "setClearOption##name##")) == NULL)																													\
			return (*this);																																											\
		renderPass->pushClearTask(color, depth, stencil);																																			\
		return (*this);																																												\
	}																																																\
																																																	\
ShadingManager &ShadingManager::pushSetClearValueTask##name##(Key<##name##> const &key, glm::vec4 const &color, float depth, uint8_t stencil)													\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key, "setClearValue##name##")) == NULL)																														\
		return (*this);																																												\
	renderPass->pushSetClearValueTask(color, depth, stencil);																																		\
	return (*this);																																													\
}																																																	\
																																																	\
ShadingManager &ShadingManager::pushSetColorMaskTask##name##(Key<##name##> const &key, glm::bvec4 const &color, GLuint index)																	\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key, "setColorMask##name##")) == NULL)																														\
		return (*this);																																												\
	renderPass->pushSetColorMaskTask(index, color);																																					\
	return (*this);																																													\
}																																																	\
																																																	\
ShadingManager &ShadingManager::pushSetStencilMaskTask##name##(Key<##name##> const &key, uint8_t front, uint8_t back)																			\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key, "setDepthStencilMask##name##")) == NULL)																													\
		return (*this);																																												\
	renderPass->pushSetStencilMaskTask(front, back);																																				\
	return (*this);																																													\
}																																																	\
																																																	\
ShadingManager &ShadingManager::pushSetDepthMaskTask##name##(Key<##name##> const &key, bool depth)																								\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key, "setDepthStencilMask##name##")) == NULL)																													\
		return (*this);																																												\
	renderPass->pushSetDepthMaskTask(depth);																																						\
	return (*this);																																													\
}																																																	\
																																																	\
ShadingManager &ShadingManager::pushSetTestTask##name##(Key<##name##> const &key, bool scissor, bool stencil, bool depth)																		\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key, "setDepthStencilMask##name##")) == NULL)																													\
		return (*this);																																												\
	renderPass->pushSetTestTask(scissor, stencil, depth);																																			\
	return (*this);																																													\
}																																																	\
																																																	\
ShadingManager &ShadingManager::pushSetScissorTask##name##(Key<##name##> const &key, glm::ivec4 const &area)																					\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key, "setDepthStencilMask##name##")) == NULL)																													\
		return (*this);																																												\
	renderPass->pushSetScissorTask(area);																																							\
	return (*this);																																													\
}																																																	\
																																																	\
ShadingManager &ShadingManager::pushSetStencilFunctionFrontFaceTask##name##(Key<##name##> const &key, GLenum func, int ref, uint8_t mask)														\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key, "setStencilFunctionFrontFace##name##")) == NULL)																											\
		return (*this);																																												\
	renderPass->pushSetStencilFunctionFrontFaceTask(func, ref, mask);																																\
	return (*this);																																													\
}																																																	\
ShadingManager &ShadingManager::pushSetStencilOperationFrontFaceTask##name##(Key<##name##> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass) 							\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key, "setStencilOperationFrontFace##name##")) == NULL)																										\
		return (*this);																																												\
	renderPass->pushSetStencilOperationFrontFaceTask(opStencilFail, opDepthFail, opDepthPass);																										\
	return (*this);																																													\
}																																																	\
																																																	\
ShadingManager &ShadingManager::pushSetStencilFunctionBackFaceTask##name##(Key<##name##> const &key, GLenum func, int ref, uint8_t mask)														\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key, "setStencilFunctionBackFace##name##")) == NULL)																											\
		return (*this);																																												\
	renderPass->pushSetStencilFunctionBackFaceTask(func, ref, mask);																																\
	return (*this);																																													\
}																																																	\
																																																	\
ShadingManager &ShadingManager::pushSetStencilOperationBackFaceTask##name##(Key<##name##> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)								\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key, "setStencilOperationBackFace##name##")) == NULL)																											\
		return (*this);																																												\
	renderPass->pushSetStencilOperationBackFaceTask(opStencilFail, opDepthFail, opDepthPass);																										\
	return (*this);																																													\
}																																																	\
																																																	\
ShadingManager &ShadingManager::pushSetStencilFunctionTask##name##(Key<##name##> const &key, GLenum func, int ref, uint8_t mask)																\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key, "setStencilFunction##name##")) == NULL)																													\
		return (*this);																																												\
	renderPass->pushSetStencilFunctionTask(func, ref, mask);																																		\
	return (*this);																																													\
}																																																	\
																																																	\
ShadingManager &ShadingManager::pushSetStencilOperationTask##name##(Key<##name##> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)										\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key, "setStencilOperation##name##")) == NULL)																													\
		return (*this);																																												\
	renderPass->pushSetStencilOperationTask(opStencilFail, opDepthFail, opDepthPass);																												\
	return (*this);																																													\
}																																																	\
																																																	\
ShadingManager &ShadingManager::pushSetBlendEquationTask##name##(Key<##name##> const &key, GLenum colorMode, GLenum alphaMode)																	\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key, "setBlendEquation##name##")) == NULL)																													\
		return (*this);																																												\
	renderPass->pushSetBlendEquationTask(colorMode, alphaMode);																																		\
	return (*this);																																													\
}																																																	\
																																																	\
ShadingManager &ShadingManager::pushSetBlendEquationTask##name##(Key<##name##> const &key, GLenum mode)																							\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key, "setBlendEquation##name##")) == NULL)																													\
		return (*this);																																												\
	renderPass->pushSetBlendEquationTask(mode);																																						\
	return (*this);																																													\
}																																																	\
																																																	\
ShadingManager &ShadingManager::pushSetBlendFuncTask##name##(Key<##name##> const &key, GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha)										\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key, "setBlendFunc##name##")) == NULL)																														\
		return (*this);																																												\
	renderPass->pushSetBlendFuncTask(srcRGB, destRGB, srcAlpha, destAlpha);																															\
	return (*this);																																													\
}																																																	\
																																																	\
ShadingManager &ShadingManager::pushSetBlendFuncTask##name##(Key<##name##> const &key, GLenum src, GLenum dest)																					\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key, "setBlendFunc##name##")) == NULL)																														\
		return (*this);																																												\
	renderPass->pushSetBlendFuncTask(src, dest);																																					\
	return (*this);																																													\
}																																																	\
																																																	\
ShadingManager &ShadingManager::pushSetBlendConstantTask##name##(Key<##name##> const &key, glm::vec4 const &blendColor)																			\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key, "setBlendConstant##name##")) == NULL)																													\
		return (*this);																																												\
	renderPass->pushSetBlendConstantTask(blendColor);																																				\
	return (*this);																																													\
}																																																	 \
																																																	 \
ShadingManager &ShadingManager::popTask##name##(Key<##name##> const &key)																														 \
{																																																	 \
	name *renderPass;																																											 \
	if ((renderPass = get##name##(key, "popTask##name##")) == NULL)																																  \
		return (*this);																																												  \
	renderPass->popTask();																																											  \
	return (*this);																																													  \
}