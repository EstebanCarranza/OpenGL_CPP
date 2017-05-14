#ifndef ecAGUA
#define ecAGUA

/*
Creado por Arix XDcs (ec)
Forma de imprementación:
Inicializar:
ecAguaX.Generar(maAgua, "Agua.bmp", 0.5, 0.5, 0.001, 0.0001);

Dibujar:
ecAguaX.Dibujar(maAgua);
*/

#include <GL/glut.h>
#include <GL/SOIL.h>
class ecAgua
{
	public:
	struct vertices
	{
		int id;
		vertices *anterior;
		GLfloat XYZ[4][3];
		vertices *siguiente;
	};

	private:
	enum aVertices
	{
		aInicio,
		aMover,
		aFinal
	};
	
	GLuint	ATextura[1];			// Storage For One Texture ( NEW )
	int CargarTextura(char path[])                                    // Load Bitmaps And Convert To Textures
	{
		/* load an image file directly as a new OpenGL texture */
		ATextura[0] = SOIL_load_OGL_texture
			(
			path,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y
			);

		if (ATextura[0] == 0)
			return false;

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, ATextura[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
	void HabilitarTextura(bool text, GLuint textura[])
	{
		if (text)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textura[0]);
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}
	}
	void HabilitarTransparencia(bool Ttrans, float TRed, float TGreen, float TBlue, float TAlpha)
	{
		if (Ttrans)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(TRed, TGreen, TBlue, TAlpha);
		}
		else
		{
			glDisable(GL_BLEND);
			glColor3f(TRed, TGreen, TBlue);
		}
			
	}
	void CrearVertices(vertices *maRoot[3], float xx, float yy, float zz)
	{
		//Crea un puntero nuevo para la lista doblemente ligada que contendrá los vertices de un mismo objeto
		vertices *nuevaCord = new vertices;

		// ENCADENAMIENTO DE PUNTEROS (GENERACION DE LA LISTA DOBLEMENTE LIGADA)
		//Si mover es nulo existe entonces...
		if (!(maRoot[aMover]))
		{
			//PRIMER REGISTRO
			(maRoot[aInicio]) = (maRoot[aMover]) = nuevaCord;
			nuevaCord->anterior = nuevaCord->siguiente = NULL;
			nuevaCord->id = 1;
		}
		else
		{
			//LOS DEMAS REGISTROS

			(maRoot[aMover])->siguiente = nuevaCord;
			nuevaCord->anterior = maRoot[aMover];

			(maRoot[aFinal]) = (maRoot[aMover]) = nuevaCord;
			(maRoot[aMover])->siguiente = NULL;
			(maRoot[aMover])->id = ((maRoot[aMover])->anterior)->id + 1;


		}

		//LLENADO DE INFORMACION

		(maRoot[aMover])->XYZ[0][0] = xx + 0.01;
		(maRoot[aMover])->XYZ[0][1] = yy;
		(maRoot[aMover])->XYZ[0][2] = zz - 0.01;

		(maRoot[aMover])->XYZ[1][0] = xx;
		(maRoot[aMover])->XYZ[1][1] = yy;
		(maRoot[aMover])->XYZ[1][2] = zz - 0.01;

		(maRoot[aMover])->XYZ[2][0] = xx;
		(maRoot[aMover])->XYZ[2][1] = yy;
		(maRoot[aMover])->XYZ[2][2] = zz;

		(maRoot[aMover])->XYZ[3][0] = xx + 0.01;
		(maRoot[aMover])->XYZ[3][1] = yy;
		(maRoot[aMover])->XYZ[3][2] = zz;

	}
	float Ola = 0;
	bool SubirBajarOla = false;
	float OlaIncremento = 0.0001;
	float altura = 0.001;
	float tile = 1.0f;
	void Oleaje(float limite, float *incremento)
	{
		//limite = 0.001
		if (Ola < limite && !SubirBajarOla)
		{
			Ola += *incremento; //0.0001
		}
		else
		{
			SubirBajarOla = true;

			if (Ola > -limite && SubirBajarOla)
			{
				Ola -= *incremento;
			}
			else
			{
				SubirBajarOla = false;
			}

		}
	}
	
	public:
		void Generar(vertices **VerticesAgua, char Textura[], float largo, float ancho, float alturaOla, float velocidadOla)
	{
		this->altura = alturaOla;
		this->OlaIncremento = velocidadOla;
		CargarTextura(Textura);
		for (float ax = 0.01; ax <= largo; ax += 0.01)
		for (float a = 0; a < ancho; a += 0.01)
		{
			ecAguaX.CrearVertices(VerticesAgua, a, 0, ax);
		}
		/* 
		ecAguaX.CargarTextura("Texturas\\agua.bmp");
		for (float ax = 0.01; ax <= 0.5; ax += 0.01)
		for (float a = 0; a < 0.5; a += 0.01)
		{
			ecAguaX.Generar(maAgua, a, 0, ax);
		}*/
	}
	void Dibujar(vertices *maRoot[3])
	{
		//0.001 y 0.0001
		Oleaje(altura, &OlaIncremento);
		maRoot[aMover] = maRoot[aInicio];

		if (maRoot[aMover])

		if (maRoot[aMover]->anterior == NULL && maRoot[aMover]->siguiente == NULL)
		{
			//codigo
			glColor3f(maRoot[aMover]->id*0.01, maRoot[aMover]->id*0.01, maRoot[aMover]->id*0.01);
			glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, tile); glVertex3f(maRoot[aMover]->XYZ[0][0], maRoot[aMover]->XYZ[0][1], maRoot[aMover]->XYZ[0][2]);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(maRoot[aMover]->XYZ[1][0], maRoot[aMover]->XYZ[1][1], maRoot[aMover]->XYZ[1][2]);
			glTexCoord2f(tile, tile); glVertex3f(maRoot[aMover]->XYZ[2][0], maRoot[aMover]->XYZ[2][1], maRoot[aMover]->XYZ[2][2]);
			glTexCoord2f(tile, 0.0f); glVertex3f(maRoot[aMover]->XYZ[3][0], maRoot[aMover]->XYZ[3][1], maRoot[aMover]->XYZ[3][2]);

			glEnd();
		}
		else
		while (maRoot[aMover]->siguiente != NULL || maRoot[aMover]->anterior != NULL)
		{
			if (maRoot[aMover]->siguiente == NULL)
				break;
			else
			{

				glPushMatrix();
				HabilitarTransparencia(true, 0.5, 0.5, 0.5, 0.5);
				HabilitarTextura(true, &ATextura[0]);

				//glColor3f(1, 1, 1);
				glBegin(GL_POLYGON);


				if (maRoot[aMover]->id % 2)
				{
					//PUNTO DEL QUAD
					glTexCoord2f(0.0f, tile); glVertex3f(maRoot[aMover]->XYZ[0][0], maRoot[aMover]->XYZ[0][1] + Ola, maRoot[aMover]->XYZ[0][2]);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(maRoot[aMover]->XYZ[1][0], maRoot[aMover]->XYZ[1][1] - Ola, maRoot[aMover]->XYZ[1][2]);
					glTexCoord2f(tile, tile); glVertex3f(maRoot[aMover]->XYZ[2][0], maRoot[aMover]->XYZ[2][1] + Ola, maRoot[aMover]->XYZ[2][2]);
					glTexCoord2f(tile, 0.0f); glVertex3f(maRoot[aMover]->XYZ[3][0], maRoot[aMover]->XYZ[3][1] - Ola, maRoot[aMover]->XYZ[3][2]);
				}
				else
				{
					//PUNTO DEL QUAD
					glTexCoord2f(0.0f, tile); glVertex3f(maRoot[aMover]->XYZ[0][0], maRoot[aMover]->XYZ[0][1] - Ola, maRoot[aMover]->XYZ[0][2]);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(maRoot[aMover]->XYZ[1][0], maRoot[aMover]->XYZ[1][1] + Ola, maRoot[aMover]->XYZ[1][2]);
					glTexCoord2f(tile, tile); glVertex3f(maRoot[aMover]->XYZ[2][0], maRoot[aMover]->XYZ[2][1] - Ola, maRoot[aMover]->XYZ[2][2]);
					glTexCoord2f(tile, 0.0f); glVertex3f(maRoot[aMover]->XYZ[3][0], maRoot[aMover]->XYZ[3][1] + Ola, maRoot[aMover]->XYZ[3][2]);
				}



				glEnd();
				HabilitarTextura(false, ATextura);
				HabilitarTransparencia(false, 1, 1, 1, 1);
				glPopMatrix();

				glColor3f(1, 1, 1);
				maRoot[aMover] = maRoot[aMover]->siguiente;
			}
	}
}
}ecAguaX;

ecAgua::vertices *maAgua[3] = { NULL, NULL, NULL };
#endif