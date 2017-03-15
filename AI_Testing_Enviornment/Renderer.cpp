#include "stdafx.h"
#include <iostream>
#include "Renderer.h"


using namespace std;

static int *gfxPrimitivesPolyIntsGlobal = NULL;
static int gfxPrimitivesPolyAllocatedGlobal = 0;

int _gfxPrimitivesCompareInt(const void *a, const void *b)
{
	return (*(const int *)a) - (*(const int *)b);
}

int hline(SDL_Renderer * renderer, Sint16 x1, Sint16 x2, Sint16 y)
{
         return SDL_RenderDrawLine(renderer, x1, y, x2, y);;
}



Renderer::Renderer() :sdl_renderer(NULL)
{

}

SDL_Renderer * Renderer::getRenderer()
{
	return sdl_renderer;
}

void Renderer::present() { //swap buffers
	SDL_RenderPresent(sdl_renderer);
}

void Renderer::clear(const Colour& col) {
	SDL_SetRenderDrawColor(sdl_renderer, col.r, col.g, col.b, col.a);
	SDL_RenderClear(sdl_renderer);
}

void Renderer::drawImage(SDL_Surface* img)
{
	SDL_Texture* ImageTexture = SDL_CreateTextureFromSurface(sdl_renderer, img);
	SDL_RenderCopy(sdl_renderer, ImageTexture, NULL, NULL);
	SDL_DestroyTexture(ImageTexture);
}

void Renderer::drawBox2DPolygon(b2PolygonShape * polygonShape, float angle)
{
	SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
	int lenght = (int)polygonShape->GetVertexCount();

	SDL_Point* points = new SDL_Point[lenght + 1];


	for (int i = 0; i < lenght; i++)
	{
		Camera2D::Point worldPoint;
		float verticesPosX = polygonShape->GetVertex(i).x;
		float verticesPosY = polygonShape->GetVertex(i).y;

		float s = sin(angle);
		float c = cos(angle);

		// translate point back to origin:
		verticesPosX -= 0;
		verticesPosY -= 0;

		// rotate point
		float xnew = verticesPosX* c - verticesPosY * s;
		float ynew = verticesPosX * s + verticesPosY * c;

		// translate point back:
		verticesPosX = xnew + 0;
		verticesPosY = ynew + 0;

		worldPoint.x = verticesPosX;
		worldPoint.y = verticesPosY;
		worldPoint = m_camera->worldToScreen(worldPoint);
		points[i].x = worldPoint.x;
		points[i].y = worldPoint.y;
	}

	points[lenght].y = points[0].y;
	points[lenght].x = points[0].x;

	SDL_RenderDrawLines(sdl_renderer, points, lenght + 1);
}

int  Renderer::drawFilledPoly(b2Body * body)
{
	b2PolygonShape* polygonShape = (b2PolygonShape*)body->GetFixtureList()->GetShape();

	SDL_Renderer * renderer = sdl_renderer;
	int n = polygonShape->GetVertexCount();
	Sint16 vx[16];
	Sint16 vy[16];

	Camera2D::Point worldPoint;
	for (int i = 0; i < n; i++)
	{
		worldPoint.x = polygonShape->GetVertex(i).x;
		worldPoint.y = polygonShape->GetVertex(i).y;
		worldPoint = m_camera->worldToScreen(worldPoint);
		vx[i] = worldPoint.x;
		vy[i] = worldPoint.y;
	}
	
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint8 a = 255;
	int **polyInts = NULL;
	int *polyAllocated = NULL;


	int result;
	int i;
	int y, xa, xb;
	int miny, maxy;
	int x1, y1;
	int x2, y2;
	int ind1, ind2;
	int ints;
	int *gfxPrimitivesPolyInts = NULL;
	int *gfxPrimitivesPolyIntsNew = NULL;
	int gfxPrimitivesPolyAllocated = 0;

	/*
	* Vertex array NULL check
	*/
	if (vx == NULL) {
		return (-1);
	}
	if (vy == NULL) {
		return (-1);
	}

	/*
	* Sanity check number of edges
	*/
	if (n < 3) {
		return -1;
	}

	/*
	* Map polygon cache
	*/
	if ((polyInts == NULL) || (polyAllocated == NULL)) {
		/* Use global cache */
		gfxPrimitivesPolyInts = gfxPrimitivesPolyIntsGlobal;
		gfxPrimitivesPolyAllocated = gfxPrimitivesPolyAllocatedGlobal;
	}
	else {
		/* Use local cache */
		gfxPrimitivesPolyInts = *polyInts;
		gfxPrimitivesPolyAllocated = *polyAllocated;
	}

	/*
	* Allocate temp array, only grow array
	*/
	if (!gfxPrimitivesPolyAllocated) {
		gfxPrimitivesPolyInts = (int *)malloc(sizeof(int) * n);
		gfxPrimitivesPolyAllocated = n;
	}
	else {
		if (gfxPrimitivesPolyAllocated < n) {
			gfxPrimitivesPolyIntsNew = (int *)realloc(gfxPrimitivesPolyInts, sizeof(int) * n);
			if (!gfxPrimitivesPolyIntsNew) {
				if (!gfxPrimitivesPolyInts) {
					free(gfxPrimitivesPolyInts);
					gfxPrimitivesPolyInts = NULL;
				}
				gfxPrimitivesPolyAllocated = 0;
			}
			else {
				gfxPrimitivesPolyInts = gfxPrimitivesPolyIntsNew;
				gfxPrimitivesPolyAllocated = n;
			}
		}
	}

	/*
	* Check temp array
	*/
	if (gfxPrimitivesPolyInts == NULL) {
		gfxPrimitivesPolyAllocated = 0;
	}

	/*
	* Update cache variables
	*/
	if ((polyInts == NULL) || (polyAllocated == NULL)) {
		gfxPrimitivesPolyIntsGlobal = gfxPrimitivesPolyInts;
		gfxPrimitivesPolyAllocatedGlobal = gfxPrimitivesPolyAllocated;
	}
	else {
		*polyInts = gfxPrimitivesPolyInts;
		*polyAllocated = gfxPrimitivesPolyAllocated;
	}

	/*
	* Check temp array again
	*/
	if (gfxPrimitivesPolyInts == NULL) {
		return(-1);
	}

	/*
	* Determine Y maxima
	*/
	miny = vy[0];
	maxy = vy[0];
	for (i = 1; (i < n); i++) {
		if (vy[i] < miny) {
			miny = vy[i];
		}
		else if (vy[i] > maxy) {
			maxy = vy[i];
		}
	}

	/*
	* Draw, scanning y
	*/
	result = 0;
	for (y = miny; (y <= maxy); y++) {
		ints = 0;
		for (i = 0; (i < n); i++) {
			if (!i) {
				ind1 = n - 1;
				ind2 = 0;
			}
			else {
				ind1 = i - 1;
				ind2 = i;
			}
			y1 = vy[ind1];
			y2 = vy[ind2];
			if (y1 < y2) {
				x1 = vx[ind1];
				x2 = vx[ind2];
			}
			else if (y1 > y2) {
				y2 = vy[ind1];
				y1 = vy[ind2];
				x2 = vx[ind1];
				x1 = vx[ind2];
			}
			else {
				continue;
			}
			if (((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2))) {
				gfxPrimitivesPolyInts[ints++] = ((65536 * (y - y1)) / (y2 - y1)) * (x2 - x1) + (65536 * x1);
			}
		}

		qsort(gfxPrimitivesPolyInts, ints, sizeof(int), _gfxPrimitivesCompareInt);

		/*
		* Set color
		*/
		result = 0;
		result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
		result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);

		for (i = 0; (i < ints); i += 2) {
			xa = gfxPrimitivesPolyInts[i] + 1;
			xa = (xa >> 16) + ((xa & 32768) >> 15);
			xb = gfxPrimitivesPolyInts[i + 1] - 1;
			xb = (xb >> 16) + ((xb & 32768) >> 15);
			result |= hline(renderer, xa, xb, y);
		}
	}

	return (result);
}

void Renderer::drawTexture(SDL_Texture* img, Rect _dst)
{
	SDL_Rect dst;
	_dst = cameraTransform(_dst);
	dst.x = (int)_dst.pos.x;
	dst.y = (int)_dst.pos.y;
	dst.w = (int)_dst.size.w;
	dst.h = (int)_dst.size.h;
	SDL_RenderCopy(sdl_renderer, img, NULL, &dst);
}

void Renderer::drawHud(SDL_Texture* img, Rect _dst)
{
	SDL_Rect dst;
	dst.x = (int)_dst.pos.x;
	dst.y = (int)_dst.pos.y;
	dst.w = (int)_dst.size.w;
	dst.h = (int)_dst.size.h;
	SDL_RenderCopy(sdl_renderer, img, NULL, &dst);
}

void Renderer::drawTexture(SDL_Texture* img, Rect _src, Rect _dst)
{
	SDL_Rect dst;
	_dst = cameraTransform(_dst);
	dst.x = (int)_dst.pos.x;
	dst.y = (int)_dst.pos.y;
	dst.w = (int)_dst.size.w;
	dst.h = (int)_dst.size.h;

	SDL_Rect src;
	src.x = (int)_src.pos.x;
	src.y = (int)_src.pos.y;
	src.w = (int)_src.size.w;
	src.h = (int)_src.size.h;
	SDL_RenderCopy(sdl_renderer, img, &src, &dst);
}

void Renderer::drawImage(SDL_Surface* img, Rect rec)
{
	rec = cameraTransform(rec);
	SDL_Point objCentre;
	objCentre.x = rec.size.w / 2;

	SDL_Rect sdlRec;
	sdlRec.h = -(int)rec.size.h;
	sdlRec.w = (int)rec.size.w;
	sdlRec.x = (int)rec.pos.x;
	sdlRec.y = (int)rec.pos.y - sdlRec.h;

	objCentre.y = sdlRec.h / 2;
	SDL_Texture* ImageTexture = SDL_CreateTextureFromSurface(sdl_renderer, img);
	SDL_RenderCopyEx(sdl_renderer, ImageTexture, NULL, &sdlRec, 0, &objCentre, SDL_FLIP_NONE);
}

void Renderer::drawLine(Vector2D _start, Vector2D _end, Colour c)
{
	SDL_SetRenderDrawColor(sdl_renderer, c.r, c.g, c.b, c.a);
	auto start = m_camera->worldToScreen(Camera2D::Point(_start.x, _start.y));
	auto end = m_camera->worldToScreen(Camera2D::Point(_end.x, _end.y));
	SDL_RenderDrawLine(sdl_renderer, start.x, start.y, end.x, end.y);
}

void Renderer::drawTextureWithAngle(SDL_Texture* img, Rect _src, Rect _dst, float angle)
{
	SDL_Rect dst;
	_dst = cameraTransform(_dst);
	dst.x = (int)_dst.pos.x;
	dst.y = (int)_dst.pos.y;
	dst.w = (int)_dst.size.w;
	dst.h = (int)_dst.size.h;

	SDL_Rect src;
	src.x = (int)_src.pos.x;
	src.y = (int)_src.pos.y;
	src.w = (int)_src.size.w;
	src.h = (int)_src.size.h;
	SDL_RenderCopyEx(sdl_renderer, img, &src, &dst, angle, NULL, SDL_FLIP_NONE);
}

void Renderer::drawTextureWithAngle(SDL_Texture * img, Rect _dst, float angle)
{
	SDL_Rect dst;
	_dst = cameraTransform(_dst);
	dst.x = (int)_dst.pos.x;
	dst.y = (int)_dst.pos.y;
	dst.w = (int)_dst.size.w;
	dst.h = (int)_dst.size.h;

	SDL_RenderCopyEx(sdl_renderer, img, NULL, &dst, angle, NULL, SDL_FLIP_NONE);
}

void Renderer::drawTextureWithAngleHud(SDL_Texture* img, Rect _src, Rect _dst, float angle)
{
	SDL_Rect dst;
	dst.x = (int)_dst.pos.x;
	dst.y = (int)_dst.pos.y;
	dst.w = (int)_dst.size.w;
	dst.h = (int)_dst.size.h;

	SDL_Rect src;
	src.x = (int)_src.pos.x;
	src.y = (int)_src.pos.y;
	src.w = (int)_src.size.w;
	src.h = (int)_src.size.h;
	SDL_RenderCopyEx(sdl_renderer, img, &src, &dst, angle, NULL, SDL_FLIP_NONE);
}

void Renderer::drawTextureWithAngleHud(SDL_Texture * img, Rect _dst, float angle)
{
	SDL_Rect dst;
	dst.x = (int)_dst.pos.x;
	dst.y = (int)_dst.pos.y;
	dst.w = (int)_dst.size.w;
	dst.h = (int)_dst.size.h;

	SDL_RenderCopyEx(sdl_renderer, img, NULL, &dst, angle, NULL, SDL_FLIP_NONE);
}


void Renderer::drawBox2DBody(b2Body * body)
{
	SDL_SetRenderDrawColor(sdl_renderer, 255, 0, 0, 255);
	for (b2Fixture* b2Fixture = body->GetFixtureList(); b2Fixture != 0; b2Fixture = b2Fixture->GetNext())
	{
		b2Shape::Type shapeType = b2Fixture->GetType();
		
		if (shapeType == b2Shape::e_polygon)
		{
			b2PolygonShape* polygonShape = (b2PolygonShape*)b2Fixture->GetShape();
			drawBox2DPolygon(polygonShape, b2Fixture->GetBody()->GetAngle());
		}
	}
}

/**Destroys SDL_Window and SDL_Renderer*/
void Renderer::destroy() 
{
	SDL_DestroyRenderer(sdl_renderer);
	SDL_DestroyWindow(window);
}

Renderer::~Renderer()
{
}

bool Renderer::init(const Vector2D& winSize, const char* title, Camera2D::Camera* cam)
{
	m_camera = cam;
	int e = SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2
	windowSize = winSize;
	if (e != 0) {
		// problem with SDL?...
		std::cout << "Could not init SDL: " << SDL_GetError() << std::endl;
		return false;
	}

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		title,							   // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		(int)winSize.w,                    // width, in pixels
		(int)winSize.h,                    // height, in pixels
		SDL_WINDOW_OPENGL                  // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		std::cout << "Could not create window: " << SDL_GetError() << std::endl;
		return false;
	}

	//Initialize PNG loading
	/*int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
	std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
	return false;
	}*/

	sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (sdl_renderer == NULL) {
		// In the case that the renderer could not be made...
		std::cout << "Could not create renderer: " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

void Renderer::setNewCamera(Camera2D::Camera * newCam)
{
	m_camera = newCam;
}

void Renderer::drawRectOutline(const Rect& r, const Colour& c)
{
	Rect tRect = cameraTransform(r);
	SDL_SetRenderDrawColor(sdl_renderer, c.r, c.g, c.b, c.a);
	SDL_Rect sr;
	sr.h = tRect.size.h;
	sr.w = tRect.size.w;
	sr.x = tRect.pos.x;
	sr.y = tRect.pos.y;
	SDL_RenderDrawRect(sdl_renderer, &sr);
}

void Renderer::drawRect(const Rect& r, const Colour& c) 
{
	Rect tRect = cameraTransform(r);
	SDL_SetRenderDrawColor(sdl_renderer, c.r, c.g, c.b, c.a);
	SDL_Rect sr;
	sr.h = tRect.size.h;
	sr.w = tRect.size.w;
	sr.x = tRect.pos.x;
	sr.y = tRect.pos.y;
	SDL_RenderFillRect(sdl_renderer, &sr);
}

void Renderer::drawRectHud(const Rect& r, const Colour& c)
{
	Rect tRect = r;
	SDL_SetRenderDrawColor(sdl_renderer, c.r, c.g, c.b, c.a);
	SDL_Rect sr;
	sr.h = tRect.size.h;
	sr.w = tRect.size.w;
	sr.x = tRect.pos.x;
	sr.y = tRect.pos.y;
	SDL_RenderFillRect(sdl_renderer, &sr);
}

Rect Renderer::cameraTransform(Rect r)
{
	SDL_Rect rect = m_camera->worldToScreen(CustomRect(r.pos.x, r.pos.y, r.size.w, r.size.h));
	
	return Rect(rect.x, rect.y, rect.w, rect.h);
}