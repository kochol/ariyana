#include "net3d.h"
#include "3d.h"
#include "3d/Node3D.hpp"
#include "net/PropertyReplicator.hpp"

void AddNode3dProperty(void* _obj, Node3dHandle* _node, char* _propertyName)
{
    const union { Node3dHandle c{}; ari::en::ComponentHandle<ari::en::Node3D> cpp; } node = { *_node };
    reinterpret_cast<ari::net::PropertyReplicator*>(_obj)->AddProperty(node.cpp, _propertyName);
}
