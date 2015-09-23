#include <Utils/ScreenPosToWorldRay.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

	void screenPosToWorldRay(
		int mouseX,
		int mouseY,
        int screenWidth,
		int screenHeight,
        const glm::mat4 &viewMatrix,
        const glm::mat4 &projectionMatrix,
        glm::vec3& out_origin,
        glm::vec3& out_direction)
	{
        glm::vec4 lRayStart_NDC(
                ((float)mouseX/(float)screenWidth  - 0.5f) * 2.0f,
                ((float)mouseY/(float)screenHeight - 0.5f) * 2.0f,
                -1.0,
                1.0f
        );
        glm::vec4 lRayEnd_NDC(
                ((float)mouseX/(float)screenWidth  - 0.5f) * 2.0f,
                ((float)mouseY/(float)screenHeight - 0.5f) * 2.0f,
                0.0,
                1.0f
        );

        glm::mat4 InverseProjectionMatrix = glm::inverse(projectionMatrix);        
        glm::mat4 InverseViewMatrix = glm::inverse(viewMatrix);
        
        glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera/=lRayStart_camera.w;
        glm::vec4 lRayStart_world  = InverseViewMatrix       * lRayStart_camera; lRayStart_world /=lRayStart_world.w;
        glm::vec4 lRayEnd_camera   = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera  /=lRayEnd_camera.w;
        glm::vec4 lRayEnd_world    = InverseViewMatrix       * lRayEnd_camera;   lRayEnd_world   /=lRayEnd_world.w;

        glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
        lRayDir_world = glm::normalize(lRayDir_world);

        out_origin = glm::vec3(lRayStart_world);
        out_direction = glm::normalize(lRayDir_world);
	}
