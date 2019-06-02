#pragma once

#include "../Common.h"
#include "../collision/Shape.h"
#include "ContactManager.h"
#include "Island.h"
#include <map>

namespace Forth
{

	namespace Physics
	{

		// This listener is used to gather information about two shapes colliding. This
		// can be used for game logic and sounds. Physics objects created in these
		// Callbacks will not be reported until the following frame. These Callbacks
		// can be called frequently, so make them efficient.
		struct IContactListener
		{
			virtual void BeginContact(Contact *contact) = 0;
			virtual void EndContact(Contact *contact) = 0;
		};

		// This class represents general queries for points, AABBs and Raycasting.
		// ReportShape is called the moment a valid shape is found. The return
		// value of ReportShape controls whether to continue or stop the query.
		// By returning only true, all shapes that fulfill the query will be re-
		// ported.
		typedef void (*QueryCallback)(const Shape &);

		class Scene
		{
		  public:
			Island island;
			ContactManager contactManager;
			::std::stack<Body *> stack;
			::std::vector<Body *> bodies;
			::std::map<int, Shape *> shapes;

			Scene();

			// Run the simulation forward in time by dt
			void Step(float Dt);

			void CollectRelatedBodies(Body *seed);

			void CreateBody(Body *body);

			// Frees a body, removes all shapes associated with the body and frees
			// all shapes and contacts associated and attached to this body.
			void RemoveBody(Body *body);

			// Removes all bodies from the scene.
			void ClearBodies();

			//// Sets the listener to report collision start/end. Provides the user
			//// with a pointer to an Contact. The Contact
			//// holds pointers to the two shapes involved in a collision, and the
			//// two bodies connected to each shape. The ContactListener will be
			//// called very often, so it is recommended for the funciton to be very
			//// efficient. Provide a NULL pointer to remove the previously set
			//// listener.
			//void SetContactListener(ContactListener listener)
			//{
			//    contactManager.ContactListener = listener;
			//}

			//struct SceneQueryAABBWrapper : ITreeCallback
			//{
			//    bool TreeCallback(int id)
			//    {
			//        Shape shape = broadPhase.Tree.GetShape(id);

			//        if (Bounds4.IsIntersecting(Aabb, shape.ComputeAABB(shape.body.GetTransform())))
			//        {
			//            return cb.ReportShape(shape);
			//        }

			//        return true;
			//    }

			//    QueryCallback cb;
			//    BroadPhase broadPhase;
			//    Bounds4 Aabb;
			//};

			//struct SceneQueryPointWrapper : ITreeCallback
			//{
			//    bool TreeCallback(int id)
			//    {
			//        Shape shape = broadPhase.Tree.GetShape(id);

			//        if (shape.TestPoint(shape.body.GetTransform(), Point))
			//        {
			//            return cb.ReportShape(shape);
			//        }

			//        return true;
			//    }

			//    QueryCallback cb;
			//    BroadPhase broadPhase;
			//    Vector4 Point;
			//};

			/// <summary>
			/// Query the world to find any shapes intersecting a ray.
			/// </summary>
			void QueryRaycast(RaycastHit4 &rayCast);
		};
	} // namespace Physics
} // namespace Forth
