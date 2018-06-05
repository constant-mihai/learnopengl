/****************************************** 

* File Name : includes/Camera.hpp

* Creation Date : 21-03-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License : 

******************************************/

/**
 * Purpose
 */

#ifndef _LOGL_CAMERA_H_
#define _LOGL_CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

/**
 * ******************************************************
 * @brief Camera class
 * ******************************************************
**/
class Camera {
    public: /* Constructors */
        Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
        {
            pos_            = pos;
            forward_        = glm::vec3(0.0f, 0.0f, -1.0f);                 /* Forward is towards -z */
            direction_      = pos_ + forward_;                              /* Default camera dir */
            up_             = glm::vec3(0.0f, 1.0f, 0.0f);                  /* Up towards +y */
            projection_     = glm::perspective(fov, aspect, zNear, zFar);
            speed_ = 0;
        }

    private: /* Methods */

    public: /* Methods */

        /**
         * ******************************************************
         * Fix the camera direction. The camera will point to a 
         * fix point in space.
         * ******************************************************
        **/
        void fix(const glm::vec3& direction) {
            direction_ = direction;
        }

        /**
         * ******************************************************
         * Unfix the camera
         * ******************************************************
        **/
        void unfix() {
            direction_ = pos_ + forward_;
        }

        /**
         * ******************************************************
         * TODO Doc this
         * ******************************************************
        **/
        glm::mat4 move(const glm::vec3& pos) {
            pos_ = pos;
            return getViewProjection(); 
        }

        /**
         * ******************************************************
         * Getters
         * ******************************************************
        **/
        glm::vec3 getPos() { return pos_; }
        glm::vec3 getForward() { return forward_; }
        glm::vec3 getDirecton() { return direction_; }
        glm::vec3 getUp() { return up_; }
        glm::vec3 getXAxis() { return glm::normalize(glm::cross(forward_, up_)); }
        glm::mat4 getProjection() { return projection_; }
        glm::mat4 getView() { return glm::lookAt(pos_, direction_, up_); }
        glm::mat4 getViewProjection() const
        {
            return projection_ * glm::lookAt(pos_, direction_, up_);
        }

        float getSpeed() { return speed_; }

        /**
         * ******************************************************
         * Setters 
         * ******************************************************
        **/
        void setPos(glm::vec3 pos) 
        { 
            pos_ = pos;
            direction_ = pos_ + forward_;
        }

        void setForward(glm::vec3 forward)
        {
            forward_ = forward;
            direction_ = pos_ + forward_;
        }

        void setUp(glm::vec3 up)
        {
            up_ = up;
        }

        void setSpeed(float speed)
        {
            speed_ = speed;
        }

    private: /* Members */
       glm::mat4 projection_;
       glm::vec3 pos_;
       glm::vec3 forward_;
       glm::vec3 direction_;
       glm::vec3 up_;
       float speed_;
};

#endif
