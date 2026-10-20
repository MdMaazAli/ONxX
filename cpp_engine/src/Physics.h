#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

// Entity Component System(ECS) ----- > basically making a stride of all the necesssary data
// what is a stride you may ask it is nothing but an array of different data bundeled together so that it makes it easier to fetch as it can be stored inside a cache
// like in this case for player(any mesh) we need some things like its Pos,Vel or M etc if its bundeled togetehr CPU would 
// require less time to get em as we are mostlikely to use their properties together like say the Pos changes and its due to Vel
//  meaning we need both Pos and Vel to be fetched so budling them together helps (it fetches directly from L1/L2 caches)

enum class Entity{INACTIVE, ACTIVE};
enum class AgentMode{NONE, TRAINING, INFERENCE};
enum class MeshType{MESH, SPHERE, CUBE, PYRAMID};
enum class Rigidbody{YES, NO};
enum class ScriptType{PYTHON, CPP, NONE};
enum class Script{DISABLED, ENABLED};

struct AgentComponent{
    AgentMode Mode = AgentMode::NONE;
    std::string AgentModule = "None";
    std::string ScriptPath = "";
};

struct MeshComponent{
    MeshType Mesh = MeshType::SPHERE;
    // -------------------------------------- > materials 
};

// Plain Old Data(POD)
struct TransformComponent{
    Entity Is = Entity::INACTIVE;
    glm::vec3 Position;
    glm::vec3 Size;
};

struct PhysicsComponent{
    Rigidbody Rigidbody = Rigidbody::NO;
    glm::vec3 Velocity;
    float Mass;
};

struct ScriptComponent{
    ScriptType Type = ScriptType::NONE;
    std::string ScriptPath = "";
    std::string ScriptName = "None";
    Script Is = Script::ENABLED;
};

// Physics System class which handle most of the physics related stuff
class PhysicsSystem{
    public:
        static void Update(std::vector<TransformComponent>& transforms, std::vector<PhysicsComponent>& physics, float dt){
            // seq loop over the stride(clearing it out)
            for(size_t i=0; i<transforms.size(); i++){
                transforms[i].Position += physics[i].Velocity*dt;
                
                if(physics[i].Rigidbody == Rigidbody::YES){
                    if(transforms[i].Position.y>0.0f){
                        physics[i].Velocity.y -= 9.81f*dt;
                    }
                    
                    if(transforms[i].Position.y<0.0f){
                        transforms[i].Position.y = 0.0f;
                        
                        if(physics[i].Velocity.y<0.0f){
                            physics[i].Velocity.y = 0.0f;
                        }
                    }
                }
            }
        }
};

#endif // PHYSICS_H