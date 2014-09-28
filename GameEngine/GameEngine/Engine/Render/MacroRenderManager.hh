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
	RenderManager &pushSetBlendStateTask##name##(Key<##name##> const &key, int drawBuffer, bool state); \
	RenderManager &popTask##name##(Key<##name##> const &key);

#define GEN_DEC_RENDEROFFSCREEN_PUSH_TASK(name) \
	RenderManager &config##name##(Key<##name##> const &renderPass, glm::ivec4 const &rect, GLenum mode = GL_TRIANGLES, GLint sample = 1); \
	RenderManager &createBufferSamplable##name##(Key<##name##> const &key, GLenum attachement, GLenum internalFormat); \
	RenderManager &createBufferNotSamplable##name##(Key<##name##> const &key, GLenum attachement, GLenum internalFormat); \
	RenderManager &deleteBuffer##name##(Key<##name##> const &key, GLenum attachement); \
	RenderManager &pushInput##name##(Key<##name##> const &key, Key<Sampler> const &s, GLenum attachement, Key<RenderPostEffect> const &r); \
	RenderManager &pushInput##name##(Key<##name##> const &key, Key<Sampler> const &s, GLenum attachement, Key<RenderPass> const &r); \
	RenderManager &popInput##name##(Key<##name##> const &key); \
	RenderManager &pushTarget##name##(Key<##name##> const &key, GLenum attachement); \
	RenderManager &popTarget##name##(Key<##name##> const &key); \
	RenderManager &useInputBuffer##name##(Key<##name##> const &key, GLenum attachement, Key<RenderPass> const &r); \
	RenderManager &useInputBuffer##name##(Key<##name##> const &key, GLenum attachement, Key<RenderPostEffect> const &r); \
	RenderManager &pushOwnTask##name##(Key<##name##> const &key, std::function<void(LocationStorage &)> const &f);


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
} \
\
	RenderManager &RenderManager::pushSetBlendStateTask##name##(Key<##name##> const &key, int drawBuffer, bool state) \
{ \
	name *render;																																											\
	if ((render = get##name##(key)) == NULL)																													\
		return (*this);																																												\
	render->pushSetBlendTask(drawBuffer, state); 																																				\
	return (*this); \
}\
																																																	 \
RenderManager &RenderManager::popTask##name##(Key<##name##> const &key)																														 \
{																																																	 \
	name *render;																																											 \
	if ((render = get##name##(key)) == NULL)																																  \
		return (*this);																																												  \
	render->popTask();																																											  \
	return (*this);																																													  \
}

#define GEN_DEF_RENDEROFFSCREEN_PUSH_TASK(name) \
	RenderManager &RenderManager::config##name##(Key<##name##> const &key, glm::ivec4 const &rect, GLenum mode, GLint sample)							\
	{																																					\
		name *render = get##name##(key);																									\
		render->setMode(mode);																												\
		render->configRect(rect);																												\
		return (*this);																																	\
	}																																					\
																																						\
	RenderManager &RenderManager::createBufferSamplable##name##(Key<##name##> const &key, GLenum attachement, GLenum internalFormat)					\
	{																																					\
		name *render = get##name##(key);																									\
		render->createBufferSamplable(attachement, internalFormat);																			\
		return (*this);																																	\
	}																																					\
																																						\
	RenderManager &RenderManager::createBufferNotSamplable##name##(Key<##name##> const &key, GLenum attachement, GLenum internalFormat)					\
	{																																					\
		name *render = get##name##(key);																									\
		render->createBufferNotSamplable(attachement, internalFormat);																		\
		return (*this);																																	\
	}																																					\
																																						\
	RenderManager &RenderManager::deleteBuffer##name##(Key<##name##> const &key, GLenum attachement)													\
	{																																					\
		name *render = get##name##(key);																									\
		render->deleteBuffer(attachement);																									\
		return (*this);																																	\
	}																																					\
																																						\
	RenderManager &RenderManager::pushInput##name##(Key<##name##> const &key, Key<Sampler> const &s, GLenum attachement, Key<RenderPass> const &r)								\
	{																																					\
		name *render = get##name##(key);																									\
		RenderPass *renderPass = getRenderPass(r); \
		render->pushInputSampler(s, attachement, *renderPass);																								\
		return (*this);																																	\
	}																																					\
\
	RenderManager &RenderManager::pushInput##name##(Key<##name##> const &key, Key<Sampler> const &s, GLenum attachement, Key<RenderPostEffect> const &r) \
	{\
		name *render = get##name##(key); \
		RenderPostEffect *renderPostEffect = getRenderPostEffect(r);\
		render->pushInputSampler(s, attachement, *renderPostEffect); \
		return (*this);\
	}\
	\
	RenderManager &RenderManager::popInput##name##(Key<##name##> const &key)																			\
	{																																					\
		name *render = get##name##(key);																									\
		render->popInputSampler();																											\
		return (*this);																																	\
	}																																					\
																																						\
	RenderManager &RenderManager::pushTarget##name##(Key<##name##> const &key, GLenum attachement)														\
	{																																					\
		name *render = get##name##(key);																										\
		render->pushTarget(attachement);																											\
		return (*this);																																	\
	}																																					\
																																						\
	RenderManager &RenderManager::popTarget##name##(Key<##name##> const &key)																			\
	{																																					\
		name *render = get##name##(key);																										\
		render->popTarget();																														\
		return (*this);																																	\
	}																																					\
																																						\
	RenderManager &RenderManager::useInputBuffer##name##(Key<##name##> const &key, GLenum attachement, Key<RenderPass> const &r)													\
	{																																					\
		name *render = get##name##(key);\
		RenderPass *renderPass = getRenderPass(r); \
		render->useInputBuffer(attachement, *renderPass); \
		return (*this); \
	} \
	\
	RenderManager &RenderManager::useInputBuffer##name##(Key<##name##> const &key, GLenum attachement, Key<RenderPostEffect> const &r)													\
	{																																					\
		name *render = get##name##(key);																										\
		RenderPostEffect *renderPostEffect = getRenderPostEffect(r); \
		render->useInputBuffer(attachement, *renderPostEffect); \
		return (*this); \
	} \
	\
	RenderManager &RenderManager::pushOwnTask##name##(Key<##name##> const &key, std::function<void(LocationStorage &)> const &f) \
	{\
		name *render = get##name##(key); \
		render->pushOwnTask(f); \
		return (*this); \
	}