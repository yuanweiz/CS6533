#include "Entity.h"
#include <functional>
#include <vector>
using namespace std;

class Geometry::Impl{
    public:
        template <typename Func_>
        void setOnDrawCallback(const Func_& func)
        {
            func_ = func;
        }
        void onDraw(){
            func_();
        }
        Impl(GLuint vbo, GLuint ibo)
            :vertexBO(vbo), indexBO(ibo)
        {
        }
        //GLuint getVBO(){return vertexBO;}
        //GLuint getIBO(){return indexBO;}
    private:
        Transform transform;
        GLuint vertexBO;
        GLuint indexBO;
        int numIndices;
        std::function<void(void)> func_;
};
Geometry::Geometry(GLuint vbo,GLuint ibo)
    : pimpl(new Impl(vbo,ibo))
{
}
Geometry::~Geometry()
{
}
void Geometry::onDraw(){
    pimpl->onDraw();
}


//Entity
class Entity::Impl{
    public:
        Entity & getSubEntiry(int i){
            return subEntities_[i];
        }

        int addSubEntity (){
            subEntities_.push_back(Entity{});
            return subEntities_.size()-1;
        }

        void addGeometry( const Geometry & g){
            geometries_.push_back(g);
        }

        Transform & getTransform(){
            return trans_;
        }
        void draw(Matrix4 eye, Matrix4 projection){
            drawWithFrame(eye,projection,getTransform().createMatrix());
        }

    private:
        void drawWithFrame (Matrix4 eye, Matrix4 projection, Matrix4 baseFrame){
            for (auto &g:geometries_){
                g.onDraw();
                Matrix4 geometryPos = g.getTransform().createMatrix();
                
            }
        }
        Transform trans_;
        vector<Geometry> geometries_;
        vector<Entity> subEntities_;
};
