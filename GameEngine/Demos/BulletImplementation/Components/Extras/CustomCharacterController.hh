#ifndef   __CUSTOM_CHARACTER_CONTROLLER_HH__
#define   __CUSTOM_CHARACTER_CONTROLLER_HH__

#include <LinearMath/btVector3.h>
#include <BulletDynamics/Character/btCharacterControllerInterface.h>
#include <BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h>

class btCollisionShape;
class btRigidBody;
class btCollisionWorld;
class btCollisionDispatcher;
class btPairCachingGhostObject;
class btConvexShape;

class CustomCharacterController : public btCharacterControllerInterface
{
protected:
   btScalar m_halfHeight;
   
   btPairCachingGhostObject* m_ghostObject;
   btConvexShape*   m_convexShape;//is also in m_ghostObject, but it needs to be convex, so we store it here to avoid upcast
   
   btScalar m_verticalVelocity;
   btScalar m_verticalOffset;
   btScalar m_fallSpeed;
   btScalar m_jumpSpeed;
   btScalar m_maxJumpHeight;
   btScalar m_maxSlopeRadians; // Slope angle that is set (used for returning the exact value)
   btScalar m_maxSlopeCosine;  // Cosine equivalent of m_maxSlopeRadians (calculated once when set, for optimization)
   btScalar m_gravity;

   btScalar m_turnAngle;
   
   btScalar m_stepHeight;

   btScalar   m_addedMargin;//@todo: remove this and fix the code

   ///this is the desired walk direction, set by the user
   btVector3   m_walkDirection;
   btVector3   m_normalizedDirection;

   //some internal variables
   btVector3 m_currentPosition;
   btScalar  m_currentStepOffset;
   btVector3 m_targetPosition;

   ///keep track of the contact manifolds
   btManifoldArray   m_manifoldArray;

   bool m_touchingContact;
   btVector3 m_touchingNormal;

   bool  m_wasOnGround;
   bool  m_wasJumping;
   bool   m_useGhostObjectSweepTest;
   bool   m_useWalkDirection;
   btScalar   m_velocityTimeInterval;
   int m_upAxis;

   static btVector3* getUpAxisDirections();

   btVector3 computeReflectionDirection (const btVector3& direction, const btVector3& normal);
   btVector3 parallelComponent (const btVector3& direction, const btVector3& normal);
   btVector3 perpindicularComponent (const btVector3& direction, const btVector3& normal);

   bool recoverFromPenetration ( btCollisionWorld* collisionWorld);
   void stepUp (btCollisionWorld* collisionWorld);
   void updateTargetPositionBasedOnCollision (const btVector3& hit_normal, btScalar tangentMag = btScalar(0.0), btScalar normalMag = btScalar(1.0));
   void stepForwardAndStrafe (btCollisionWorld* collisionWorld, const btVector3& walkMove);
   void stepDown (btCollisionWorld* collisionWorld, btScalar dt);
public:
   CustomCharacterController(btPairCachingGhostObject* ghostObject,btConvexShape* convexShape,btScalar stepHeight, int upAxis = 1);
   ~CustomCharacterController();

   ///btActionInterface interface
   virtual void updateAction( btCollisionWorld* collisionWorld,btScalar deltaTime)
   {
      preStep ( collisionWorld);
      playerStep (collisionWorld, deltaTime);
   }
   
   ///btActionInterface interface
   void   debugDraw(btIDebugDraw* debugDrawer);

   void setUpAxis (int axis)
   {
      if (axis < 0)
         axis = 0;
      if (axis > 2)
         axis = 2;
      m_upAxis = axis;
   }

   virtual void setUpInterpolate(bool value)
   {}

   /// This should probably be called setPositionIncrementPerSimulatorStep.
   /// This is neither a direction nor a velocity, but the amount to
   ///   increment the position each simulation iteration, regardless
   ///   of dt.
   /// This call will reset any velocity set by setVelocityForTimeInterval().
   virtual void   setWalkDirection(const btVector3& walkDirection);

   /// Caller provides a velocity with which the character should move for
   ///   the given time period.  After the time period, velocity is reset
   ///   to zero.
   /// This call will reset any walk direction set by setWalkDirection().
   /// Negative time intervals will result in no motion.
   virtual void setVelocityForTimeInterval(const btVector3& velocity,
            btScalar timeInterval);


   void setTurnAngle(btScalar ang) { m_turnAngle = ang; };

   virtual void reset (btCollisionWorld* collisionWorld);
   void warp (const btVector3& origin);

   void preStep (  btCollisionWorld* collisionWorld);
   void playerStep ( btCollisionWorld* collisionWorld, btScalar dt);

   void setFallSpeed (btScalar fallSpeed)            { m_fallSpeed = btFabs(fallSpeed); }
   void setJumpSpeed (btScalar jumpSpeed)            { m_jumpSpeed = btFabs(jumpSpeed); }
   void setMaxJumpHeight (btScalar maxJumpHeight)      { m_maxJumpHeight = maxJumpHeight; }
   
   ///
   bool isJumping ()const                        { return m_wasJumping; }
   ///
   bool canJump() const                        { return onGround(); }
   
   void jump ();

   void setGravity(btScalar gravity)               { m_gravity = gravity; }
   btScalar getGravity() const                     { return m_gravity; }

   /// The max slope determines the maximum angle that the controller can walk up.
   /// The slope angle is measured in radians.
   void setMaxSlope(btScalar slopeRadians)            { m_maxSlopeRadians = slopeRadians; m_maxSlopeCosine = btCos(slopeRadians); }
   btScalar getMaxSlope() const                  { return m_maxSlopeRadians; }

   btPairCachingGhostObject* getGhostObject()         { return m_ghostObject; }
   void setUseGhostSweepTest(bool use)               { m_useGhostObjectSweepTest = use; }

   bool onGround () const                        { return m_verticalVelocity == 0.0 && m_verticalOffset == 0.0; }
};
#endif // __CUSTOM_CHARACTER_CONTROLLER_HH__