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
    Entity *me,*other;
    Body *obody;
    if ((!data)||(!context))return;
    me = (Entity *)data;
    obody = (Body *)context;
    if (entity_is_entity(obody->touch.data))
    {
        other = (Entity *)obody->touch.data;
        //slog("%s is ",other->name);
    }
   // slog("touching me.... touching youuuuuuuu");
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

void playerThink(Entity *self)
{
  if(!self)return;
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
    mgl_callback_set(&ent->body.touch,touch_callback,ent);
    return ent;
}

Entity *newSpaceShip(Vec3D position,Vec3D rotation, const char *name){
  Entity *ent;
  
  
  ent = entity_new();
  if (!ent)
  {
    return NULL;
  }
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
  

int main(int argc, char *argv[])
{
    int i;
    float r = 0;
    Space *space;
    Entity *cube1,*cube2, *player;
    char bGameLoopRunning = 1;
    
    Vec3D camTarget = {0,0,0};
    
    Vec3D cameraPosition = {0,-10,0};
    Vec3D cameraRotation = {90,0,0};
    
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
                else if (e.key.keysym.sym == SDLK_SPACE)
                {
                    cameraPosition.z++;
		    //playerPosition to match cameraPosition
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
		    vec3d_add(cameraPosition,
			      cameraPosition,
			      vec3d(
				((sin((cameraRotation.z) *DEGTORAD) * sin((cameraRotation.x) * DEGTORAD)) * 10),
				(-(cos((cameraRotation.z) *DEGTORAD) * sin((cameraRotation.x) * DEGTORAD)) * 10),
				(cos(cameraRotation.x * DEGTORAD) * 10)
			     ));
		    /*not what I need, rotates around the z axis at 0 only*
		     vec3d((-sin((cameraRotation.z + 180) * DEGTORAD) * 10),
				    (cos((cameraRotation.z + 180)* DEGTORAD) * 10),
				    cameraPosition.z)
				    */
		    //playerRotation to match cameraRotation
                }
                else if (e.key.keysym.sym == SDLK_RIGHT)
                {
		    
		    vec3d_cpy(cameraPosition, player->body.position);
                    cameraRotation.z -= 1;
		    vec3d_add(cameraPosition,
			      cameraPosition,
			      vec3d(
				((sin((cameraRotation.z) *DEGTORAD) * sin((cameraRotation.x) * DEGTORAD)) * 10),
				(-(cos((cameraRotation.z) *DEGTORAD) * sin((cameraRotation.x) * DEGTORAD)) * 10),
				(cos(cameraRotation.x * DEGTORAD) * 10)
			     ));
		      
		    //playerRotation to match cameraRotation
                }
                else if (e.key.keysym.sym == SDLK_UP)
                {
		  vec3d_cpy(cameraPosition, player->body.position);
                    cameraRotation.x += 1;
		    vec3d_add(cameraPosition,
			      cameraPosition,
			      vec3d(
				((sin((cameraRotation.z) *DEGTORAD) * sin((cameraRotation.x) * DEGTORAD)) * 10),
				(-(cos((cameraRotation.z) *DEGTORAD) * sin((cameraRotation.x) * DEGTORAD)) * 10),
				(cos(cameraRotation.x * DEGTORAD) * 10)
			      ));
		    //playerRotation to match cameraRotation
                }
                else if (e.key.keysym.sym == SDLK_DOWN)
                {
                    vec3d_cpy(cameraPosition, player->body.position);
                    cameraRotation.x -= 1;
		    vec3d_add(cameraPosition,
			      cameraPosition,
			       vec3d(
				((sin((cameraRotation.z) *DEGTORAD) * sin((cameraRotation.x) * DEGTORAD)) * 10),
				(-(cos((cameraRotation.z) *DEGTORAD) * sin((cameraRotation.x) * DEGTORAD)) * 10),
				(cos(cameraRotation.x * DEGTORAD) * 10)
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