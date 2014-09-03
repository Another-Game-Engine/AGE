#pragma once

#define GEN_DEC_RENDER_PUSH_TASK(name) \
	RenderManager &pushClearTask##name##(Key<##name##> const &key, bool color = true, bool depth = true, bool stencil = false); \
	RenderManager &pushSetClearValueTask##name##(Key<##name##> const &key, glm::vec4 const &color, float depth = 1.0f, uint8_t stencil = 0); \
	RenderManager &pushSetColorMaskTask##name##(Key<##name##> const &key, glm::bvec4 const &color, GLuint index = 0); \
	RenderManager &pushSetDepthMaskTask##name##(Key<##name##> const &key, bool depth); \
	RenderManager &pushSetStencilMaskTask##name##(Key<##name##> const &key, uint8_t front, uint8_t back); \
	RenderManager &pushSetTestTask##name##(Key<##name##> const &key, bool scissor, bool stencil, bool depth); \
	RenderManager &pushSetScissorTask##name##(Key<##name##> const &key, glm::ivec4 const &area); \
	RenderManager &pushSetStencilFunctionFrontFaceTask##name##(Key<##name##> const &key, GLenum func, int ref, uint8_t mask = 0xff); \
	RenderManager &pushSetStencilOperationFrontFaceTask##name##(Key<##name##> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass); \
	RenderManager &pushSetStencilFunctionBackFaceTask##name##(Key<##name##> const &key, GLenum func, int ref, uint8_t mask = 0xff); \
	RenderManager &pushSetStencilOperationBackFaceTask##name##(Key<##name##> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass); \
	RenderManager &pushSetStencilFunctionTask##name##(Key<##name##> const &key, GLenum func, int ref, uint8_t mask = 0xff); \
	RenderManager &pushSetStencilOperationTask##name##(Key<##name##> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass); \
	RenderManager &pushSetBlendEquationTask##name##(Key<##name##> const &key, GLenum mode); \
	RenderManager &pushSetBlendEquationTask##name##(Key<##name##> const &key, GLenum colorMode, GLenum alphaMode); \
	RenderManager &pushSetBlendFuncTask##name##(Key<##name##> const &key, GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha); \
	RenderManager &pushSetBlendFuncTask##name##(Key<##name##> const &key, GLenum src, GLenum dest); \
	RenderManager &pushSetBlendConstantTask##name##(Key<##name##> const &key, glm::vec4 const &blendPass); \
	RenderManager &popTask##name##(Key<##name##> const &key);

#define GEN_DEF_RENDER_PUSH_TASK(name) 																																								\
	RenderManager &RenderManager::pushClearTask##name##(Key<##name##> const &key, bool color, bool depth, bool stencil)																			\
	{																																																\
		##name## *renderPass;																																											\
		if ((renderPass = get##name##(key)) == NULL)																													\
			return (*this);																																											\
		renderPass->pushClearTask(color, depth, stencil);																																			\
		return (*this);																																												\
	}																																																\
																																																	\
RenderManager &RenderManager::pushSetClearValueTask##name##(Key<##name##> const &key, glm::vec4 const &color, float depth, uint8_t stencil)													\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key)) == NULL)																														\
		return (*this);																																												\
	renderPass->pushSetClearValueTask(color, depth, stencil);																																		\
	return (*this);																																													\
}																																																	\
																																																	\
RenderManager &RenderManager::pushSetColorMaskTask##name##(Key<##name##> const &key, glm::bvec4 const &color, GLuint index)																	\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key)) == NULL)																														\
		return (*this);																																												\
	renderPass->pushSetColorMaskTask(index, color);																																					\
	return (*this);																																													\
}																																																	\
																																																	\
RenderManager &RenderManager::pushSetStencilMaskTask##name##(Key<##name##> const &key, uint8_t front, uint8_t back)																			\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key)) == NULL)																													\
		return (*this);																																												\
	renderPass->pushSetStencilMaskTask(front, back);																																				\
	return (*this);																																													\
}																																																	\
																																																	\
RenderManager &RenderManager::pushSetDepthMaskTask##name##(Key<##name##> const &key, bool depth)																								\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key)) == NULL)																													\
		return (*this);																																												\
	renderPass->pushSetDepthMaskTask(depth);																																						\
	return (*this);																																													\
}																																																	\
																																																	\
RenderManager &RenderManager::pushSetTestTask##name##(Key<##name##> const &key, bool scissor, bool stencil, bool depth)																		\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key)) == NULL)																													\
		return (*this);																																												\
	renderPass->pushSetTestTask(scissor, stencil, depth);																																			\
	return (*this);																																													\
}																																																	\
																																																	\
RenderManager &RenderManager::pushSetScissorTask##name##(Key<##name##> const &key, glm::ivec4 const &area)																					\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key)) == NULL)																													\
		return (*this);																																												\
	renderPass->pushSetScissorTask(area);																																							\
	return (*this);																																													\
}																																																	\
																																																	\
RenderManager &RenderManager::pushSetStencilFunctionFrontFaceTask##name##(Key<##name##> const &key, GLenum func, int ref, uint8_t mask)														\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key)) == NULL)																											\
		return (*this);																																												\
	renderPass->pushSetStencilFunctionFrontFaceTask(func, ref, mask);																																\
	return (*this);																																													\
}																																																	\
RenderManager &RenderManager::pushSetStencilOperationFrontFaceTask##name##(Key<##name##> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass) 							\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key)) == NULL)																										\
		return (*this);																																												\
	renderPass->pushSetStencilOperationFrontFaceTask(opStencilFail, opDepthFail, opDepthPass);																										\
	return (*this);																																													\
}																																																	\
																																																	\
RenderManager &RenderManager::pushSetStencilFunctionBackFaceTask##name##(Key<##name##> const &key, GLenum func, int ref, uint8_t mask)														\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key)) == NULL)																											\
		return (*this);																																												\
	renderPass->pushSetStencilFunctionBackFaceTask(func, ref, mask);																																\
	return (*this);																																													\
}																																																	\
																																																	\
RenderManager &RenderManager::pushSetStencilOperationBackFaceTask##name##(Key<##name##> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)								\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key)) == NULL)																											\
		return (*this);																																												\
	renderPass->pushSetStencilOperationBackFaceTask(opStencilFail, opDepthFail, opDepthPass);																										\
	return (*this);																																													\
}																																																	\
																																																	\
RenderManager &RenderManager::pushSetStencilFunctionTask##name##(Key<##name##> const &key, GLenum func, int ref, uint8_t mask)																\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key)) == NULL)																													\
		return (*this);																																												\
	renderPass->pushSetStencilFunctionTask(func, ref, mask);																																		\
	return (*this);																																													\
}																																																	\
																																																	\
RenderManager &RenderManager::pushSetStencilOperationTask##name##(Key<##name##> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass)										\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key)) == NULL)																													\
		return (*this);																																												\
	renderPass->pushSetStencilOperationTask(opStencilFail, opDepthFail, opDepthPass);																												\
	return (*this);																																													\
}																																																	\
																																																	\
RenderManager &RenderManager::pushSetBlendEquationTask##name##(Key<##name##> const &key, GLenum colorMode, GLenum alphaMode)																	\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key)) == NULL)																													\
		return (*this);																																												\
	renderPass->pushSetBlendEquationTask(colorMode, alphaMode);																																		\
	return (*this);																																													\
}																																																	\
																																																	\
RenderManager &RenderManager::pushSetBlendEquationTask##name##(Key<##name##> const &key, GLenum mode)																							\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key)) == NULL)																													\
		return (*this);																																												\
	renderPass->pushSetBlendEquationTask(mode);																																						\
	return (*this);																																													\
}																																																	\
																																																	\
RenderManager &RenderManager::pushSetBlendFuncTask##name##(Key<##name##> const &key, GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha)										\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key)) == NULL)																														\
		return (*this);																																												\
	renderPass->pushSetBlendFuncTask(srcRGB, destRGB, srcAlpha, destAlpha);																															\
	return (*this);																																													\
}																																																	\
																																																	\
RenderManager &RenderManager::pushSetBlendFuncTask##name##(Key<##name##> const &key, GLenum src, GLenum dest)																					\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key)) == NULL)																														\
		return (*this);																																												\
	renderPass->pushSetBlendFuncTask(src, dest);																																					\
	return (*this);																																													\
}																																																	\
																																																	\
RenderManager &RenderManager::pushSetBlendConstantTask##name##(Key<##name##> const &key, glm::vec4 const &blendColor)																			\
{																																																	\
	name *renderPass;																																											\
	if ((renderPass = get##name##(key)) == NULL)																													\
		return (*this);																																												\
	renderPass->pushSetBlendConstantTask(blendColor);																																				\
	return (*this);																																													\
}																																																	 \
																																																	 \
RenderManager &RenderManager::popTask##name##(Key<##name##> const &key)																														 \
{																																																	 \
	name *renderPass;																																											 \
	if ((renderPass = get##name##(key)) == NULL)																																  \
		return (*this);																																												  \
	renderPass->popTask();																																											  \
	return (*this);																																													  \
}