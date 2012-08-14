#include <math.h> 

struct vertex{
	float x,y,z;
};
struct face{
	int f1,f2,f3;
};
class mesh{
private: int vnum;
private: int fnum;
private: vertex* V;
private: face* F;

public: void setVnum(int num){
		vnum=num;
		V=(vertex*)malloc(sizeof(vertex)*num);
	}
public: void setFnum(int num){
		fnum=num;
		F=(face*)malloc(sizeof(face)*num);
	}
public: void setV(int num,float x,float y,float z){
		V[num].x=x;
		V[num].y=y;
		V[num].z=z;
	}
public: void setF(int num,int f1,int f2,int f3){
		F[num].f1=f1;
		F[num].f2=f2;
		F[num].f3=f3;
	}
public: int getVnum(void){
		return vnum;
	}
public: int getFnum(void){
		return fnum;
	}
public: vertex getV(int num){
		return V[num];
	}
public: face getF(int num){
		return F[num];
	}
public: vertex getN(int num){
		vertex out;
		float x1,y1,z1,x2,y2,z2,length;
		x1=getV(getF(num).f2-1).x-getV(getF(num).f1-1).x;
		y1=getV(getF(num).f2-1).y-getV(getF(num).f1-1).y;
		z1=getV(getF(num).f2-1).z-getV(getF(num).f1-1).z;
		x2=getV(getF(num).f3-1).x-getV(getF(num).f1-1).x;
		y2=getV(getF(num).f3-1).y-getV(getF(num).f1-1).y;
		z2=getV(getF(num).f3-1).z-getV(getF(num).f1-1).z;
		out.x=y1*z2-y2*z1;
		out.y=x2*z1-x1*z2;
		out.z=x1*y2-x2*y1;
		/*length=sqrt(out.x*out.x+out.y*out.y+out.z*out.z);
		out.x=out.x/length;
		out.y=out.y/length;
		out.z=out.z/length;
		*/return out;
	}
public: void kill(void){
			free(V);
			free(F);

		}
};
