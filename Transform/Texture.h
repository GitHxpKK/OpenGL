#ifndef TEXTURE_H
#define TEXTURE_H

#include<string.h>
#include<GL\glew.h>
#include<SOIL\SOIL.h>
class Texture
{
public:

	//图片ID
	GLuint texture;
	Texture(const GLchar* texturePath,GLuint id);

	//设置环绕方式
	void setWrapping(const GLchar* mays);

	//设置过滤方式
	void setFiltering(const GLchar* mays);

	//生成纹理
	void getTexture();
	//解绑纹理对象并释放图像数据
	void releaseTexture();
	~Texture();

private:

	//图片长度值与高度值
	int width;
	int height;
	//获取图片数据变量
	unsigned char* image;


};

Texture::Texture(const GLchar* texturePath, GLuint id)
{
	glGenTextures(id, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	image = SOIL_load_image(texturePath, &width, &height, 0, SOIL_LOAD_RGB);
}
void Texture::setWrapping(const GLchar* ways){
	if (ways == "REPET"){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	
}
void Texture::setFiltering(const GLchar* ways){
	if (ways == "NEAREST"){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	if (ways == "LINER"){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

}

void Texture::getTexture(){
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
}
void Texture::releaseTexture(){
	
	SOIL_free_image_data(image);   //释放图像数据
	glBindTexture(GL_TEXTURE_2D, 0);	//解绑纹理对象
}

Texture::~Texture()
{
}
#endif