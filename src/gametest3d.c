/**
 * gametest3d
 * @license The MIT License (MIT)
 *   @copyright Copyright (c) 2015 EngineerOfLies
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *    SOFTWARE.
 */
#include "mgl_callback.h"
#include "simple_logger.h"
#include "graphics3d.h"
#include "shader.h"
#include "obj.h"
#include "vector.h"
#include "sprite.h"
#include "entity.h"
#include "space.h"

void set_camera(Vec3D position, Vec3D rotation);

void touch_callback(void *data, void *context)
{
    float temp;
    Entity *me,*other;
    Body *obody;
    if ((!data)||(!context))return;
    me = (Entity *)data;
    obody = (Body *)context;
    
    if (entity_is_entity(obody->touch.data))
    {
      other = (Entity *)obody->touch.data;
     
     if(me->state!= 1 && other->state!=1)
     {
        
	 slog("other type %i", other->type);
	  slog("me type %i", me->type);
	if(other->type == me->type)
	{
	  slog("they is the same");
	  return;
	}
	
	else{
	  if(other->health >= 0){
	  temp = other->health ;
	  slog("%s is ",other->name);
	  
	  other->health -= me->health;
	  }
	  if(me->health >= 0)
	  me->health -= temp;
	
	  
	
	  slog("here's what I think the %s health value is: %f", other->name,other->health);
	}
	    if(other->health <= 0)
	  other->state = 1;
	  if(me->health <= 0)
	  me->state = 1;
     }
    }

}

void hit_callback(void *data, void *context)
{
     Entity *me,*other;
    Body *obody;
    if ((!data)||(!context))return;
    me = (Entity *)data;
    obody = (Body *)context;
    if (entity_is_entity(obody->touch.data))
    {
        other = (Entity *)obody->touch.data;
        slog("%s is ",other->name);
	other->health-= me->health;
    }
   slog("losing health");
}

void think(Entity *self)
{
    if (!self)return;
    switch(self->state)
    {
        case 0:
            self->frame = 0;
            break;
        case 1:
            self->frame += 0.3;
            if (self->frame >= 24)self->frame = 0;
            break;
        case 2:
            self->frame -= 0.3;
            if (self->frame < 0)self->frame = 23;
            break;
    }
    self->objModel = self->objAnimation[(int)self->frame];
}

void asteroidThink(Entity *self){
 if(!self) return;
  if (self->state == 1){
    entity_free(self);
 }
  
 if((self->acceleration.x != self->body.velocity.x)
   || (self->acceleration.y != self->body.velocity.y)
   || (self->acceleration.z != self->body.velocity.z)
){
  if(self->acceleration.x <  self->body.velocity.x){
    self->body.velocity.x -= 1;
  }
  else
    self->body.velocity.x += 1;
  if(self->acceleration.y <  self->body.velocity.y){
    self->body.velocity.y -= 1;
  }
  else
    self->body.velocity.y += 1;
  if(self->acceleration.z <  self->body.velocity.z){
    self->body.velocity.z -= 1;
  }
  else
    self->body.velocity.z += 1;
  
  
 }
 
 vec3d_add(self->body.position,
	   self->body.position,
	   self->body.velocity);
}

void enemyThink(Entity *self){
 if(!self) return; 
 if (self->state == 1){
    entity_free(self);
 }
 
}

void laserThink(Entity *self){
   if(!self) return;
    if (self->state == 1){
    entity_free(self);
 }
  
 if((self->acceleration.x != self->body.velocity.x)
   || (self->acceleration.y != self->body.velocity.y)
   || (self->acceleration.z != self->body.velocity.z)
){
  if(self->acceleration.x <  self->body.velocity.x){
    self->body.velocity.x -= 1;
  }
  else
    self->body.velocity.x += 1;
  if(self->acceleration.y <  self->body.velocity.y){
    self->body.velocity.y -= 1;
  }
  else
    self->body.velocity.y += 1;
  if(self->acceleration.z <  self->body.velocity.z){
    self->body.velocity.z -= 1;
  }
  else
    self->body.velocity.z += 1;
  
  
 }
 
 vec3d_add(self->body.position,
	   self->body.position,
	   self->body.velocity);
}

void playerThink(Entity *self)
{
  if(!self)return;
  
  
  //press Space
  //Spawn Lazers
  //
  //Press C
  //Cycle Current Weapon
  //
  //Press X
  //Cycle Current Weapon
  //
  //Press Z
  //DO A BARREL ROLL!!!!!
  
  
  /*
  switch(self->state)
  {
   
    case 0: 
      //spawning?
      break;
    case 1:
      // dying?
      break;
      
    case 2: 
      //dead?
      break;
      
    case 3:
      //idle?
      break;
    case 4:
      //need to figure out player movemonet
    break;
 */
}

Vec3D normalizeVector(Vec3D temp){
   float length = sqrt((temp.x*temp.x) + (temp.y*temp.y) + (temp.z*temp.z));
   Vec3D normVec = {temp.x/length, temp.y/length, temp.z/length};
   return(normVec);
}

Vec3D getTargetVector(Vec3D tempPosition, Vec3D tempTarget)
{
  Vec3D temp = {0,0,0};
  vec3d_add(temp, tempPosition, vec3d(-tempTarget.x, -tempTarget.y, -tempTarget.z));
  
  normalizeVector(temp);
  return (temp);
}



Vec3D getCrossProduct(Vec3D temp1, Vec3D temp2){
  Vec3D  crossProduct = {temp1.y * temp2.z - temp1.z * temp2.y,
			  temp1.z*temp2.x - temp1.x*temp2.z, 
			  temp1.x* temp2.z - temp1.y*temp2.x};
 return(crossProduct);
}

Entity *newCube(Vec3D position,const char *name)
{
    Entity * ent;
    char buffer[255];
    int i;
    ent = entity_new();
    if (!ent)
    {
        return NULL;
    }
    for (i = 0; i < 24;i++)
    {
        sprintf(buffer,"models/robot/walk_bot_%06i.obj",i + 1);
        ent->objAnimation[i] = obj_load(buffer);
    }
    ent->objModel = ent->objAnimation[0];
    ent->texture = LoadSprite("models/robot/robot.png",1024,1024);
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
    ent->rotation.x = 90;
    sprintf(ent->name,"%s",name);
    ent->think = think;
    ent->state = 0;

    return ent;
}



Entity *newSpaceShip(Vec3D position,Vec3D rotation, const char *name){
  Entity *ent;
  
  
  ent = entity_new();
  if (!ent)
  {
    return NULL;
  }
  ent->type = 0;
  ent->health = 100.00;
  ent->currFireDelay = 0.0;
  ent->fireDelay = 1.0;
  ent->currDamageDelay = 0.0;
  ent->damageDelay = 4.0;
  ent->currSpecialDelay = 0.0;
  ent->specialDelay = 6.0;
  ent->weapon = 0;
  ent->maxWeapons = 2;
    ent->objModel = obj_load("models/monkey.obj");
    ent->texture = LoadSprite("models/monkey.png",1024,1024);
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
  vec3d_cpy(ent->rotation, rotation);
  sprintf(ent->name, "%s", name);
  ent->think = playerThink;
  ent->state = 0;
  mgl_callback_set(&ent->body.touch, touch_callback,ent);
  return ent;
}

Entity *newEnemy(Vec3D position, Vec3D rotation){
Entity *ent;
ent = entity_new();
  sprintf(ent->name,"%s","laser");
if(!ent)
{
  return NULL;
}
ent->type = 1;
ent->health = 40;
ent->objModel = obj_load("models/monkey.obj");
ent->texture = LoadSprite("models/cube_text.png", 1024, 1024);
vec3d_cpy(ent->body.position, position);
vec3d_cpy(ent->rotation, rotation);
cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
ent->currDamageDelay = 0.0;
ent->damageDelay = 0.5;
ent->rotation = vec3d(0,0,0);
ent->think = enemyThink;
ent->state = 0;
mgl_callback_set(&ent->body.touch, touch_callback,ent);

return ent;
}

Entity*newLaser(Vec3D position, Vec3D rotation, Vec3D acceleration, int type){
  Entity *ent;
  ent = entity_new();
  if(!ent)
  {
   return NULL; 
  }
    sprintf(ent->name,"%s","laser");
  ent->type = type;
  ent->health = 10;
  vec3d_cpy(ent->acceleration, acceleration);
  ent->objModel = obj_load("models/cube.obj");
  ent->texture = LoadSprite("models/monkey.png",1024,1024);
  vec3d_cpy(ent->body.position, position);
  vec3d_cpy(ent->rotation, rotation);
  cube_set(ent->body.bounds, -1,-1,-1,2,2,2);
  vec3d_cpy(ent->rotation, rotation);
  ent->think = laserThink;
  vec3d_cpy(ent->body.velocity, acceleration);
  ent->state = 0;
  mgl_callback_set(&ent->body.touch, touch_callback,ent);
  return ent;
}

Entity *newAsteroid(Vec3D position, Vec3D rotation, Vec3D acceleration){
 Entity *ent;
 
 ent = entity_new();
 if (!ent)
 {
   return NULL;
 }
    ent->type = 2;
    ent->health = 20.00;
    vec3d_cpy(ent->acceleration, acceleration);
    ent->objModel = obj_load("models/cube.obj");
    ent->texture = LoadSprite("models/cube_text.png",1024, 1024);
    vec3d_cpy(ent->body.position, position);
    vec3d_cpy(ent->rotation, rotation);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
    ent->currDamageDelay = 0.0;
    ent->damageDelay = 0.0;
    ent->rotation=vec3d(0,0,0);
    ent->think = asteroidThink;
    ent->state = 0;
    vec3d_cpy(ent->body.velocity, acceleration);
    mgl_callback_set(&ent->body.touch, touch_callback,ent);
    return ent;
}
  

int main(int argc, char *argv[])
{
    int i;
    float r = 0;
    Space *space;
    Entity *cube1,*cube2, *player, *asteroid, *movingAsteroid, *enemy, *laser;
    char bGameLoopRunning = 1;
    
    Vec3D camTarget = {0,0,0};
    int testing = 0;
    Vec3D cameraPosition = {0,-10,0};
    Vec3D cameraRotation = {90,0,0};
    Vec3D temporary = {0.0,0.0,0.0};
    float  directionModifier = 1.0;
    SDL_Event e;
    Obj *bgobj,*chicken;
    Sprite *bgtext;
    
    init_logger("gametest3d.log");
    if (graphics3d_init(1024,768,1,"gametest3d",33) != 0)
    {
        return -1;
    }
    model_init();
    obj_init();
    entity_init(255);
    
    chicken = obj_load("models/monkey.obj");
    bgobj = obj_load("models/mountainvillage.obj");
    bgtext = LoadSprite("models/mountain_text.png",1024,1024);
    
    
    cube1 = newCube(vec3d(cameraPosition.x,0,0),"Cubert");
    cube2 = newCube(vec3d(10,0,0),"Hobbes");
    player = newSpaceShip(camTarget,(cameraRotation), "Player");
    
    cube2->body.velocity.x = -0.1;
    
   
    
    
    
    space = space_new();
    space_set_steps(space,100);
    
    space_add_body(space,&cube1->body);
    space_add_body(space,&cube2->body);
    space_add_body(space,&player->body);
    while (bGameLoopRunning)
    {
       
        entity_think_all();
        for (i = 0; i < 100;i++)
        {
            space_do_step(space);
        }
        while ( SDL_PollEvent(&e) ) 
        {

	  camTarget = player->body.position;
	  
	 
            if (e.type == SDL_QUIT)
            {
                bGameLoopRunning = 0;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    bGameLoopRunning = 0;
                }
                else if (e.key.keysym.sym == SDLK_t){
		  if (testing == 0)
		    testing = 1;
		  else 
		    testing = 0;
		}
		if (testing == 1){
		 if (e.key.keysym.sym == SDLK_r){
		  //spawn asteroid; 
		   asteroid = newAsteroid(vec3d(camTarget.x, camTarget.y + 20, camTarget.z), vec3d(0,0,0), vec3d(0,0,0));
		   space_add_body(space,&asteroid->body);
		 }
		 if (e.key.keysym.sym == SDLK_f){
		  //spawn moving asteroid; 
		   movingAsteroid = newAsteroid(vec3d(camTarget.x, camTarget.y + 20, camTarget.z), vec3d(0,20,0), vec3d(0,2,0));
		   space_add_body(space,&movingAsteroid->body);
		 }
		 if (e.key.keysym.sym == SDLK_v){
		  //spawn enemy;
		   enemy = newEnemy(vec3d(camTarget.x, camTarget.y + 20, camTarget.z), vec3d(0,20,0));
		   space_add_body(space,&enemy->body);
		   
		 }
		}
                if (e.key.keysym.sym == SDLK_SPACE)
                {temporary =  vec3d(
                            -sin(cameraRotation.z * DEGTORAD) *2,
                            cos(cameraRotation.z * DEGTORAD) * 2,
                            -cos(cameraRotation.x * DEGTORAD) * 2
                        );
                   laser = newLaser(camTarget, cameraRotation, temporary, player->type); 
		   
		   space_add_body(space,&laser->body);
                }
                else if (e.key.keysym.sym == SDLK_z)
                {
                    cameraPosition.z--;
		    //playerPosition to match cameraPosition
                }
                else if (e.key.keysym.sym == SDLK_w)
                {
                    vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            -sin(cameraRotation.z * DEGTORAD),
                            cos(cameraRotation.z * DEGTORAD),
                            -cos(cameraRotation.x * DEGTORAD)
                        ));
		     
		    vec3d_add(
		      camTarget,
		      camTarget,
		      vec3d(
                            -sin(cameraRotation.z * DEGTORAD),
                            cos(cameraRotation.z * DEGTORAD),
                            -cos(cameraRotation.x * DEGTORAD)
                        ));
		    vec3d_cpy(
		      player->body.position,
		      camTarget);
                }
                else if (e.key.keysym.sym == SDLK_s)
                {
                     vec3d_add(
		       cameraPosition,
		       cameraPosition,
		       vec3d(
			 sin(cameraRotation.z * DEGTORAD),
			     -cos(cameraRotation.z * DEGTORAD),
			     cos(cameraRotation.x * DEGTORAD)
		       ));
		    vec3d_add(
		      camTarget,
		      camTarget,
		      vec3d(
                            sin(cameraRotation.z * DEGTORAD),
                            -cos(cameraRotation.z * DEGTORAD),
                            cos(cameraRotation.x * DEGTORAD)
                        ));
		    
		    vec3d_cpy(
		      player->body.position,
		      camTarget);
                }
                else if (e.key.keysym.sym == SDLK_d)
                {
		   
                    vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            cos(cameraRotation.z * DEGTORAD),
                            sin(cameraRotation.z * DEGTORAD),
                            0
                        ));
		    vec3d_add(
		      camTarget,
		      camTarget,
		      vec3d(
                            cos(cameraRotation.z * DEGTORAD),
                            sin(cameraRotation.z * DEGTORAD),
                            0
                        ));
		    
		    vec3d_cpy(
		      player->body.position,
		      camTarget);
		    
		    //playerPosition to match cameraPosition
                }
                else if (e.key.keysym.sym == SDLK_a)
                {
                    vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            -cos(cameraRotation.z * DEGTORAD),
                            -sin(cameraRotation.z * DEGTORAD),
                            0
                        ));
		    vec3d_add(
		      camTarget,
		      camTarget,
		      vec3d(
                            -cos(cameraRotation.z * DEGTORAD),
                            -sin(cameraRotation.z * DEGTORAD),
                            0
                        ));
		    
		    vec3d_cpy(
		      player->body.position,
		      camTarget);
		    //playerPosition to match cameraPosition
		    
                }
                else if (e.key.keysym.sym == SDLK_LEFT)
                {
		    vec3d_cpy(cameraPosition, player->body.position);
                    cameraRotation.z += 1;
		    player->rotation.y += 1;
		    vec3d_add(cameraPosition,
			      cameraPosition,
			      vec3d(
				((sin((cameraRotation.z) *DEGTORAD) * sin((cameraRotation.x) * DEGTORAD)) * 15),
				(-(cos((cameraRotation.z) *DEGTORAD) * sin((cameraRotation.x) * DEGTORAD)) * 15),
				(cos(cameraRotation.x * DEGTORAD) * 15)
			     ));
		    
		   
		    /*not what I need, rotates around the z axis at 0 only*
		     vec3d((-sin((cameraRotation.z + 180) * DEGTORAD) * 15),
				    (cos((cameraRotation.z + 180)* DEGTORAD) * 15),
				    cameraPosition.z)
				    */
		    //playerRotation to match cameraRotation
                }
                else if (e.key.keysym.sym == SDLK_RIGHT)
                {
		    
		    vec3d_cpy(cameraPosition, player->body.position);
                    cameraRotation.z -= 1;
		    player->rotation.y -= 1;
		    vec3d_add(cameraPosition,
			      cameraPosition,
			      vec3d(
				((sin((cameraRotation.z) *DEGTORAD) * sin((cameraRotation.x) * DEGTORAD)) *15),
				(-(cos((cameraRotation.z) *DEGTORAD) * sin((cameraRotation.x) * DEGTORAD)) * 15),
				(cos(cameraRotation.x * DEGTORAD) * 15)
			     ));
		      
		    //playerRotation to match cameraRotation
                }
                else if (e.key.keysym.sym == SDLK_UP)
                {
		  vec3d_cpy(cameraPosition, player->body.position);
                    cameraRotation.x += 1;
		    player->rotation.x += 1;
		    vec3d_add(cameraPosition,
			      cameraPosition,
			      vec3d(
				((sin((cameraRotation.z) *DEGTORAD) * sin((cameraRotation.x) * DEGTORAD)) * 15),
				(-(cos((cameraRotation.z) *DEGTORAD) * sin((cameraRotation.x) * DEGTORAD)) * 15),
				(cos(cameraRotation.x * DEGTORAD) * 15)
			      ));
		    //playerRotation to match cameraRotation
                }
                else if (e.key.keysym.sym == SDLK_DOWN)
                {
                    vec3d_cpy(cameraPosition, player->body.position);
                    cameraRotation.x -= 1;
		    player->rotation.x -= 1;
		    vec3d_add(cameraPosition,
			      cameraPosition,
			       vec3d(
				((sin((cameraRotation.z) *DEGTORAD) * sin((cameraRotation.x) * DEGTORAD)) * 15),
				(-(cos((cameraRotation.z) *DEGTORAD) * sin((cameraRotation.x) * DEGTORAD)) * 15),
				(cos(cameraRotation.x * DEGTORAD) * 15)
			     ));
		    //playerRotation to match cameraRotation
                }
                else if (e.key.keysym.sym == SDLK_n)
                {
                    cube1->state ++;
                    if (cube1->state >= 3)cube1->state = 0;
                }
            }
        }

        graphics3d_frame_begin();
        
        glPushMatrix();
        set_camera(
            cameraPosition,
            cameraRotation);
	
        glPushMatrix();
        entity_draw_all();
        glPushMatrix();
        glTranslatef(-5,0,0);
        obj_draw(
            chicken,
            vec3d(0,0,0),
            vec3d(0,0,0),
            vec3d(1,1,1),
            vec4d(1,0,0,1),
            NULL
        );
        glPushMatrix();
        
        glTranslatef(0,1,0);
        glScalef(0.5,0.5,0.5);
        
        glRotatef(45,0.0,0.0,1.00);
        obj_draw(
            chicken,
            vec3d(0,0,0),
            vec3d(0,0,0),
            vec3d(1,1,1),
            vec4d(0,1,0,1),
            NULL
        );        
        glPushMatrix();
        glRotatef(45,0.0,1.0,0.0);
        obj_draw(
            chicken,
            vec3d(0,0,0),
            vec3d(0,0,0),
            vec3d(0.5,0.5,0.5),
            vec4d(0,0,1,1),
            NULL
        );        
        
        glPopMatrix();
        glPopMatrix();
        glPopMatrix();
        glPopMatrix();
        obj_draw(
            bgobj,
            vec3d(0,0,2),
            vec3d(90,90,0),
            vec3d(5,5,5),
            vec4d(1,1,1,1),
            bgtext
        );
        
        if (r > 360)r -= 360;
        glPopMatrix();
        /* drawing code above here! */
        graphics3d_next_frame();
    } 
    return 0;
}

void set_camera(Vec3D position, Vec3D rotation)
{
    glRotatef(-rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(-rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(-rotation.z, 0.0f, 0.0f, 1.0f);
    glTranslatef(-position.x,
                 -position.y,
                 -position.z);
    
   
}

/*eol@eof*/