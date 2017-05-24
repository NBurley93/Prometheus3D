#include "Font.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/gtc/matrix_transform.hpp>

void Font::Init(std::string filePath, int pSize) {
	FT_Library ft;
	FT_Init_FreeType(&ft);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	FT_Face fontFace;
	if (FT_New_Face(ft, filePath.c_str(), 0, &fontFace)) {
		std::printf("Freetype has encountered a problem loading a font!\n");
		perror(filePath.c_str());
	}
	FT_Set_Pixel_Sizes(fontFace, 0, pSize);

	//Generate characters
	for (GLubyte c = 0; c < 128; c++) {
		if (FT_Load_Char(fontFace, c, FT_LOAD_RENDER)) {
			std::printf("Freetype encountered a problem on glyph[%d] for %s\n", c, filePath.c_str());
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, fontFace->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		FontCharacter character = {
			texture,
			glm::ivec2(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows),
			glm::ivec2(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top),
			fontFace->glyph->advance.x
		};
		mFontCharacters.insert(std::pair<GLchar, FontCharacter>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(fontFace);
	FT_Done_FreeType(ft);

	//Buffers
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Font::setColor(glm::vec3 nColor) {
	mTextColor = nColor;
}

void Font::RenderText(GLShader& shdr, std::string text, GLfloat x, GLfloat y, GLfloat scale) {
	glActiveTexture(GL_TEXTURE0);
	glUniform3f(shdr.getUniformLocation("textColor"), mTextColor.x / 255.0f, mTextColor.y / 255.0f, mTextColor.z / 255.0f);
	glUniform1i(shdr.getUniformLocation("text"), 0);
	glBindVertexArray(mVAO);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		//Render text
		FontCharacter ch = mFontCharacters[*c];

		GLfloat xpos =  x + ch.Bearing.x * scale;
		GLfloat ypos = y + (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * (-scale);

		GLfloat vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos, ypos, 0.0, 1.0 },
			{ xpos + w, ypos, 1.0, 1.0 },

			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 0.0 }
		};

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.Advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}