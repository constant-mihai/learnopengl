/****************************************** 

* File Name : includes/Transform.hpp

* Creation Date : 12-03-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License : 

******************************************/

/**
 * Purpose
 */

#ifndef _LOGL_TRANSFORM_H_
#define _LOGL_TRANSFORM_H_

/**
 * ******************************************************
 * @brief Transform class
 * ******************************************************
**/
class Transform {
    public: /* Constructors */
        /**
         * ******************************************************
         * @brief Default Tranform contructor
         * ******************************************************
        **/
        Transform() :
            model_(1.0f), translate_(), scale_(), rotate_()
        {
            model();
        }

        /**
         * ******************************************************
         * @brief Single value constructor
         *
         * @param[in] translate distance
         * @param[in] scale size
         * @param[in] angles 
         * ******************************************************
        **/
        Transform(float translateDist, float scaleSize, glm::vec3 angles):
            model_(1.0f),
            translate_ (translateDist),
            scale_ (scaleSize),
            rotate_ (angles)
        {
            model();
        }

        /**
         * ******************************************************
         * @brief Vector constructor
         *
         * @param[in] translate distance
         * @param[in] scale size
         * @param[in] angles 
         * ******************************************************
        **/
        Transform(glm::vec3 translateDist, glm::vec3 scaleSize, glm::vec3 angles):
            model_(1.0f),
            translate_ (translateDist),
            scale_ (scaleSize),
            rotate_ (angles)
        {
            model();
        }

        /**
         * ******************************************************
         * @brief MathOps class
         * ******************************************************
        **/
        class MatOps
        {
            public:
                MatOps():
                    mat_(1.0f) {};

                MatOps(float val):
                    mat_(val) {};

                MatOps(glm::mat4 mat) { mat_ = mat; };

                MatOps& operator=(const MatOps & rhs)
                {
                    if (&rhs != this)
                    {
                        mat_ = rhs.getMatRef();
                    }
                    return *this;
                }

                MatOps operator*(const MatOps & rhs)
                {
                    const glm::mat4 & rhs_mat = rhs.getMatRef();
                    return MatOps((mat_ * rhs_mat)); 
                }

                const glm::mat4 * getMat() const { return &mat_; };
                const glm::mat4 & getMatRef() const { return mat_; };
             protected:
                glm::mat4 mat_;
        };

        /**
         * ******************************************************
         * @brief Translate subclass
         * ******************************************************
        **/
        class Translate : public MatOps {
            public:
                Translate(): MatOps(1.0f), vec_(0, 0, 0) {};
                Translate(float pos): MatOps(1.0f), vec_(pos, pos, pos) { translate(mat_, vec_); };
                Translate(glm::vec3 vec): MatOps(1.0f), vec_(vec) { translate(mat_, vec_); };

                const glm::vec3 * getVec() const { return &vec_; };

                Translate& operator=(const Translate& rhs)
                {
                    if (&rhs != this)
                    {
                        setVec(*(rhs.getVec()));
                    }
                    return *this;
                }

                void setVec(glm::vec3 vec) 
                { 
                    vec_= vec;
                    translate(mat_, vec_);
                };

            private:
                glm::vec3       vec_;
        };

        /**
         * ******************************************************
         * @brief Scale subclass
         * ******************************************************
        **/
        class Scale : public MatOps {
            public:
                Scale(): MatOps(1.0f), vec_(0, 0, 0) {};
                Scale(float pos): MatOps(1.0f), vec_(pos, pos, pos) { scale(mat_, vec_); };
                Scale(glm::vec3 vec): MatOps(1.0f), vec_(vec) { scale(mat_, vec_); };

                const glm::vec3 * getVec() const { return &vec_; };

                Scale& operator=(const Scale& rhs)
                {
                    if (&rhs != this)
                    {
                        setVec(*(rhs.getVec()));
                    }
                    return *this;
                }

                void setVec(glm::vec3 vec) 
                { 
                    vec_= vec;
                    scale(mat_, vec_);
                };

            private:
                glm::vec3       vec_;
        };

        /**
         * ******************************************************
         * @brief Rotate subclass
         * ******************************************************
        **/
        class Rotate : public MatOps {
            public:
                Rotate(): MatOps(1.0f), vec_(0, 0, 0) {};
                Rotate(glm::vec3 vec): MatOps(1.0f), vec_(vec) { rotQuat(mat_, vec_); };

                const glm::vec3 * getVec() const { return &vec_; };

                Rotate& operator=(const Rotate& rhs)
                {
                    if (&rhs != this)
                    {
                        setVec(*(rhs.getVec()));
                    }
                    return *this;
                }

                void setVec(glm::vec3 vec) 
                { 
                    vec_= vec;
                    rotQuat(mat_, vec_);
                };

                /**
                 * ******************************************************
                 * @brief Creates a rotation matrix using euler angles 
                 * ******************************************************
                **/
                static void rotEuler(glm::mat4 & tf, float degrees, glm::vec3 axis)
                {
                    tf = glm::rotate(glm::mat4(1.0f), glm::radians(degrees), axis);
                }

                /**
                 * ******************************************************
                 * @brief Creates a rotation using quaternions 
                 * ******************************************************
                **/
                static void rotQuat(glm::mat4 & tf, float degrees, glm::vec3 axis)
                {
                    tf = glm::toMat4(glm::angleAxis(glm::radians(degrees), axis));
                }

                /**
                 * ******************************************************
                 * @brief Creates a rotation using quaternions 
                 * ******************************************************
                **/
                static void rotQuat(glm::mat4 & tf, glm::vec3 angles)
                {
                    glm::quat qt;
                    qt = glm::quat(glm::radians(angles));
                    tf = glm::toMat4(qt);
                }
            private:
                glm::vec3       vec_;
        };

    private: /* Methods */
    public: /* Methods */

        Transform& operator=(const Transform& rhs)
        {
            if (&rhs != this)
            {
                model_ = *(rhs.getModel());
                translate_ = rhs.getTranslate();
                scale_ = rhs.getScale();
                rotate_ = rhs.getRotate();
            }

            return *this;
        }

        glm::mat4 operator*(const Transform& rhs)
        {
            return (model_ * *(rhs.getModel()));
        }

        glm::mat4 operator*(const glm::mat4& rhs)
        {
            return (model_ * rhs);
        }

        /**
         * ******************************************************
         * @brief Getters
         * ******************************************************
        **/
        const glm::mat4 * getModel() const { return &model_; };
        const float * getModelFloat() const { return &model_[0][0]; };
        const Translate & getTranslate() const { return translate_; };
        const Scale & getScale() const { return scale_; };
        const Rotate & getRotate() const { return rotate_; };


        /**
         * ******************************************************
         * @brief Calculates the model using scale, rotation and translation
         * ******************************************************
        **/
        void model()
        {
            //TODO this can get slow.
            //Both options will create new matrices on multiplication.
            //But MatOps will also copy them in the ctor.
            //_AND_ it will copy them once more on model_ definition.
            //MatOps op = translate_ * rotate_ * scale_;
            //model_ = (*(op.getMat()));

            const glm::mat4 & tla = translate_.getMatRef();
            const glm::mat4 & sca = scale_.getMatRef();
            const glm::mat4 & rot = rotate_.getMatRef();

            model_ = tla * sca * rot;
        }

        /**
         * ******************************************************
         * @brief Setters 
         * ******************************************************
        **/
        void set(float translate, float scale, glm::vec3 angles) 
        {
            translate_.setVec(glm::vec3(translate, translate, translate));
            scale_.setVec(glm::vec3(scale, scale, scale));
            rotate_.setVec(angles);
            model();
        }

        void set(glm::vec3 translate, glm::vec3 scale, glm::vec3 angles) 
        {
            translate_.setVec(translate);
            scale_.setVec(scale);
            rotate_.setVec(angles);
            model();
        }

        void setTranslate(float pos)      
        { 
            translate_.setVec(glm::vec3(pos, pos, pos));
            model();
        };

        void setTranslate(glm::vec3 pos)  
        { 
            translate_.setVec(glm::vec3(pos));
            model();
        };

        void setScale(float scale)      
        { 
            scale_.setVec(glm::vec3(scale, scale, scale));
            model();
        };

        void setScale(glm::vec3 scale)  
        { 
            scale_.setVec(glm::vec3(scale)); 
            model();
        };

        void setRotate(glm::vec3 angles)  
        {
            rotate_.setVec(glm::vec3(angles)); 
            model();
        };

        /**
         * ******************************************************
         * @brief Recalculates the model by reapplying the
         * translate, rotate and scale matrices
         * ******************************************************
        **/
        void remodel()
        {
            MatOps op = translate_ * rotate_ * scale_;
            model_ = op.getMatRef() * model_;
        }

        void retranslate() 
        {
            model_ = translate_.getMatRef() * model_;
        };
        void rescale() 
        {
            model_ = scale_.getMatRef() * model_; 
        };
        void rerotate() 
        {
            model_ = rotate_.getMatRef() * model_;
        };

        /**
         * ******************************************************
         * @brief Creates a translate matrix 
         *
         * @param[out] tf           - transform matrix
         * @param[in]  dist         - distance to translate 
         * ******************************************************
        **/
        static void translate(glm::mat4 & tf, float dist)
        {
            tf = glm::translate(glm::mat4(1.0f), glm::vec3(dist, dist, dist));
        }

        /**
         * ******************************************************
         * @brief Creates a translate matrix 
         *
         * @param[out] tf           - transform matrix
         * @param[in]  dist         - distance to translate 
         * ******************************************************
        **/
        static void translate(glm::mat4 & tf, glm::vec3 dist)
        {
            tf = glm::translate(glm::mat4(1.0f), dist);
        }

        /**
         * ******************************************************
         * @brief Creates a scaling matrix 
         *
         * @param[out] tf           - transform matrix
         * @param[in]  size         - scaling size  
         * ******************************************************
        **/
        static void scale(glm::mat4 & tf, float size)
        {
            tf = glm::scale(glm::mat4(1.0f), glm::vec3(size, size, size)); 
        }

        /**
         * ******************************************************
         * @brief Creates a scale matrix 
         *
         * @param[out] tf           - transform matrix
         * @param[in]  size         - scaling size  
         * ******************************************************
        **/
        static void scale(glm::mat4 & tf, glm::vec3 size)
        {
            tf = glm::scale(glm::mat4(1.0f), size); 
        }


    private: /* Members */

        glm::mat4 model_;       /* The model matrix */
        Translate translate_;   /* The translate matrix */
        Scale scale_;           /* The scale matrix */
        Rotate rotate_;         /* The rotate matrix */
};

#endif
