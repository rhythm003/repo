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
private: vertex* VN;
private: vertex Center;
private: vertex Translate;
private: vertex Rotate;
private: float rot_angle;

public: void setVnum(int num){
		vnum=num;
		free(V);
		V=(vertex*)malloc(sizeof(vertex)*num);
	}
public: void setFnum(int num){
		fnum=num;
		free(F);
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
		length=sqrt(out.x*out.x+out.y*out.y+out.z*out.z);
		out.x=out.x/length;
		out.y=out.y/length;
		out.z=out.z/length;
		return out;
	}
public: vertex getVN(int num){
		return VN[num];
		}
public: void setVN(void){
		free(VN);
		VN=(vertex*)malloc(sizeof(vertex)*vnum);
		float length;
		vertex out;
		/*for(int j=0;j<vnum;j++){
			out.x=0;out.y=0;out.z=0;
			for(int i=0;i<fnum;i++){		
				if(F[i].f1-1==j||F[i].f2-1==j||F[i].f3-1==j){
					out.x+=getN(i).x;
					out.y+=getN(i).y;
					out.z+=getN(i).z;
				}
			}
			length=sqrt(out.x*out.x+out.y*out.y+out.z*out.z);
			out.x=out.x/length;
			out.y=out.y/length;
			out.z=out.z/length;
			VN[j].x=out.x;
			VN[j].y=out.y;
			VN[j].z=out.z;
		}*/
		for(int i=0;i<vnum;i++){
			VN[i].x=0;
			VN[i].y=0;
			VN[i].z=0;
		}
		for(int i=0;i<fnum;i++){
			VN[getF(i).f1-1].x+=getN(i).x;
			VN[getF(i).f1-1].y+=getN(i).y;
			VN[getF(i).f1-1].z+=getN(i).z;
			VN[getF(i).f2-1].x+=getN(i).x;
			VN[getF(i).f2-1].y+=getN(i).y;
			VN[getF(i).f2-1].z+=getN(i).z;
			VN[getF(i).f3-1].x+=getN(i).x;
			VN[getF(i).f3-1].y+=getN(i).y;
			VN[getF(i).f3-1].z+=getN(i).z;
		}
		for(int i=0;i<vnum;i++){
			length=sqrt(VN[i].x*VN[i].x+VN[i].y*VN[i].y+VN[i].z*VN[i].z);
			VN[i].x=VN[i].x/length;
			VN[i].y=VN[i].y/length;
			VN[i].z=VN[i].z/length;
		}
		
		}
public: void kill(void){
			free(V);
			free(F);

		}
public: void setCenter(void){
			vertex out;
			out.x=0;
			out.y=0;
			out.z=0;
			for(int i=0;i<vnum;i++){
				out.x+=getV(i).x;
				out.y+=getV(i).y;
				out.z+=getV(i).z;
			}
			out.x=out.x/(float)vnum;
			out.y=out.y/(float)vnum;
			out.z=out.z/(float)vnum;
			Center.x=out.x;
			Center.y=out.y;
			Center.z=out.z;
		}
public: vertex getCenter(){
			return Center;
		}
public: void initPose(void){
			Translate.x=0;Translate.y=0;Translate.z=0;
			Rotate.x=0;Rotate.y=1;Rotate.z=0;
			rot_angle=0;
		}
public: void setTrans(float x,float y,float z){
			Translate.x=x;
			Translate.y=y;
			Translate.z=z;
		}
public: void setRotate(float angle,float x,float y,float z){
			rot_angle=angle;
			Rotate.x=x;
			Rotate.y=y;
			Rotate.z=z;
		}
public: vertex getTrans(void){
			vertex out;
			out.x=Translate.x;
			out.y=Translate.y;
			out.z=Translate.z;
			return out;
		}
public: vertex getRotate(void){
			vertex out;
			out.x=Rotate.x;
			out.y=Rotate.y;
			out.z=Rotate.z;
			return out;
		}
public: float getRotate_angle(void){
			return rot_angle;
		}
public: void copy(mesh in){
			setVnum(in.getVnum());
			setFnum(in.getFnum());
			for(int i=0;i<vnum;i++){
				setV(i,in.getV(i).x,in.getV(i).y,in.getV(i).z);
			}
			for(int i=0;i<fnum;i++){
				setF(i,in.getF(i).f1,in.getF(i).f2,in.getF(i).f3);
			}
			setVN();
			setCenter();
			setTrans(in.getTrans().x,in.getTrans().y,in.getTrans().z);
			setRotate(in.getRotate_angle(),in.getRotate().x,in.getRotate().y,in.getRotate().z);
		}
};
