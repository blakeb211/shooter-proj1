#include "globals.h"
#include "builder.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
using namespace std;
using namespace sf;
using Vec2 = Vector2<float>;
//-----------------------------------------
// Constructor 
// ----------------------------------------
Entity::Entity() : id{entityCount++}, destroyed{false} {}
void Entity::update(FrameTime ftStep) { /* check for collision */ /* update pos */  }

void Entity::collideWith(EntityType et, unsigned int ivox) { }
//-----------------------------------------
// Destructor 
// ----------------------------------------
Entity::~Entity() { }  
//-----------------------------------------
// Set position of entity as a whole 
// ----------------------------------------
void Entity::setPos(Vec2 pos) {
    this->move(pos - this->pos);
}
//-----------------------------------------
// Return position of entity as a whole
// ----------------------------------------
Vec2& Entity::getPos() { return pos; }
//-----------------------------------------
// Move all voxels by an offset
// ----------------------------------------
void Entity::move(Vec2 offset) { 
    this->pos += offset;
    for(auto &v : vox) { 
        v.move(offset); }
}
//-----------------------------------------
// Return the voxel vector  
// ----------------------------------------
vector<Voxel>& Entity::getVox() { return vox; }
//-----------------------------------------
// Entity health is the voxel count 
// ----------------------------------------
size_t Entity::getHealth() const { return vox.size(); }
//-----------------------------------------
// Return the entity id  
// ----------------------------------------
const size_t& Entity::getId() const { return id; }
//-----------------------------------------
// Static method: Return the entity with the given id 
// ----------------------------------------
shared_ptr<Entity> Entity::withId(size_t id) {
    return *find_if(begin(G::entity),end(G::entity),
            [&id](const shared_ptr<Entity> & e) 
            { return e->id == id; });
}

//-----------------------------------------
// Static method: Set health of each voxel 
// ----------------------------------------
void Entity::setVoxelHealth(Entity & e, optional<unsigned int> health) { 
    if (e.vox.size() == 0) 
        throw exception("tried to set health of entity with 0 voxels");
    for(auto & v : e.vox) {
        v.health = health;
    }
}

// out of line static initializers
size_t Entity::entityCount = 0;

// Bullet types
Bullet::Bullet(Vec2 vv) : vel{vv} {
    o_type = EntityType::Bullet;
}

void Bullet::update(FrameTime ftStep){ 
    move(ftStep*vel);  
    // mark for destruction if go off screen
    if (pos.x < 0 || pos.x > G::screenWidth || pos.y < 0 || pos.y > G::screenHeight) {
        destroyed = true;
    }
}

B1::B1(Vec2 pos) : Bullet({0.f,-0.6f}) {
    Builder::build_B1(vox);
    setPos(pos);
    Entity::setVoxelHealth(*this, 1);
}

B2::B2(Vec2 pos) : Bullet({0.f,-0.6f}) {
    Builder::build_B2(vox);
    setPos(pos);
    Entity::setVoxelHealth(*this, 2);
}
B3::B3(Vec2 pos) : Bullet({0.f,-0.6f}) {
    Builder::build_B3(vox);
    setPos(pos);
    // set bullet 2 voxel health
}

// Player methods
Player::Player(Vec2 pos) : mTimerMax{75.f}, mTimer{0.f}, mCanShoot{false}  {
    o_type = EntityType::Player;
    Builder::build_player(vox);
    setPos(pos);
    Entity::setVoxelHealth(*this, 5);
}

void Player::update(FrameTime ftStep) {
    mTimer += ftStep; 
    if (mTimer > mTimerMax) {
        mTimer = 0.f;
        mCanShoot = true;
    }
}

// Enemy types
Enemy::Enemy() : currPathPoint{0} {
    o_type = EntityType::Enemy;
}

void Enemy::update(FrameTime ftStep) {
    auto _pos = getPos();
    auto pathPoint = path[currPathPoint];
    float dist2goal = sqrt(pow(_pos.x - pathPoint.x,2) + pow(_pos.y - pathPoint.y,2));
    if (dist2goal < 3.f) {
        if (currPathPoint == path.size() - 1)
        {
            // move from last path point to first path point
            currPathPoint = 0; 
        }
        else {
            currPathPoint++;
        }
        pathPoint = path[currPathPoint];
    }
    auto moveDir = pathPoint - _pos;
    float length = sqrt(pow(moveDir.x,2) + pow(moveDir.y,2));
    auto unitVec = Vec2(moveDir.x / length, moveDir.y / length);
    float slowDownFactor = 0.2f;
    // move in direction of next goal position
    move(unitVec * slowDownFactor * ftStep); 
    // move by dvel, which dampens to 0 over time, as well
    move(dvel);
}

E1::E1(Vec2 pos) : Enemy() {
    Builder::build_E1(vox);
    setPos(pos);
    Entity::setVoxelHealth(*this, 1);
}

E2::E2(Vec2 pos) : Enemy() {
    Builder::build_E2(vox);
    setPos(pos);
    Entity::setVoxelHealth(*this, 2);
    //set all voxels to a fixed health value
}

E3::E3(Vec2 pos) : Enemy() {
    Builder::build_E3(vox);
    setPos(pos);
    Entity::setVoxelHealth(*this, 3);
    //set all voxels to a fixed health value
}

E4::E4(Vec2 pos) : Enemy() {
    Builder::build_E4(vox);
    setPos(pos);
    Entity::setVoxelHealth(*this, 4);
    //set all voxels to a fixed health value
}

// Wall types
Wall1::Wall1(Vec2 start, Vec2 end) {
    o_type = EntityType::Wall1;
    Builder::build_wall1(start, end, vox);
    // This is a bouncy wall so health == nullopt
    Entity::setVoxelHealth(*this, nullopt);
}

void Wall1::update(FrameTime ftStep) {
    // check for collisions
}

void Wall2::update(FrameTime ftStep) {
    // check for collisions
    
}
