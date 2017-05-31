#pragma once
#include "icg_helper.h"

class FrameBuffer {

    private:
        int width_;
        int height_;
        GLuint framebuffer_object_id_;
        GLuint depth_render_buffer_id_;
        GLuint color_texture_id_;

    public:
        // warning: overrides viewport!!
        void Bind() {
            glViewport(0, 0, width_, height_);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object_id_);
            // Updated number of buffers
            const GLenum buffers[] = { GL_COLOR_ATTACHMENT0};
            glDrawBuffers(1 /*length of buffers[]*/, buffers);
        }

        void Unbind() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        int Init(int image_width, int image_height, bool use_interpolation = true, GLint internalFormat = GL_R32F, GLenum format = GL_RED ) {
            this->width_ = image_width;
            this->height_ = image_height;

            // create color attachment 0
            {
                glGenTextures(1, &color_texture_id_);
                glBindTexture(GL_TEXTURE_2D, color_texture_id_);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

                if(use_interpolation){
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                } else {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                }

                // create texture for the color attachment
                // see Table.2 on
                // khronos.org/opengles/sdk/docs/man3/docbook4/xhtml/glTexImage2D.xml
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width_, height_, 0,
                             format, GL_UNSIGNED_BYTE, NULL);
                // how to load from buffer
            }
            // tie it all together
            {
                glGenFramebuffers(1, &framebuffer_object_id_);
                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object_id_);
                glFramebufferTexture2D(GL_FRAMEBUFFER,
                                       GL_COLOR_ATTACHMENT0 /*location = 0*/,
                                       GL_TEXTURE_2D, color_texture_id_,
                                       0 /*level*/);

                if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
                    GL_FRAMEBUFFER_COMPLETE) {
                    cerr << "!!!ERROR: Framebuffer not OK :(" << endl;
                }
                glBindFramebuffer(GL_FRAMEBUFFER, 0); // avoid pollution
            }


            return color_texture_id_;
        }
        void Clear() {
            glViewport(0, 0, width_, height_);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object_id_);
                glDrawBuffer(GL_COLOR_ATTACHMENT0);
                glClearColor(0.5, 0.5, 0.5, 1.0);
                glClear(GL_COLOR_BUFFER_BIT);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        void Cleanup() {
            glDeleteTextures(1, &color_texture_id_);
            glBindFramebuffer(GL_FRAMEBUFFER, 0 /*UNBIND*/);
            glDeleteFramebuffers(1, &framebuffer_object_id_);
        }

};
