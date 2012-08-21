#pragma once
#include <Windows.h>	//add
#include <gl/gl.h>		//add
#include <gl/glu.h>		//add
#include <iostream>
#include "mesh.h"
#include <cv.h>
#include <highgui.h>
#include <math.h>

#define PI 3.14159265

namespace viewer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices; //add
	using namespace std;
 
	[DllImport("opengl32.dll")]		//add
	extern HDC wglSwapBuffers( HDC hdc );		//add
	/// <summary>
	/// Form1 的摘要
	///
	/// 警告: 如果您變更這個類別的名稱，就必須變更與這個類別所依據之所有 .resx 檔案關聯的
	///          Managed 資源編譯器工具的 'Resource File Name' 屬性。
	///          否則，這些設計工具
	///          將無法與這個表單關聯的當地語系化資源
	///          正確互動。
	/// </summary>
	bool click=false,key=false;
	GLdouble Eyex=0,Eyey=0,Eyez=100;
	GLdouble Focusx=0,Focusy=0,Focusz=0,movex,movey,movez,rotx,roty,rotz;
	GLdouble ViewLeft=-100,ViewRight=100,ViewBottom=-100,ViewTop=100,ViewNear=-1000,ViewFar=1000;
	//GLdouble ViewLeft=-2,ViewRight=2,ViewBottom=-2,ViewTop=2,ViewNear=-20,ViewFar=20;
	double Mousex,Mousey;
	double rot_angle=5,move_step=5;
	mesh Model1,Model2;
	bool Model1_load=false,Model2_load=false;
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
			OpenGLInit();
		}
		void OpenGLInit(void)
		{
			// Get Handle
			m_hWnd = (HWND)this->showPan->Handle.ToInt32();
			m_hDC = GetDC( m_hWnd );
			//WGL::wglSwapBuffers((unsigned int)m_hDC);
			wglSwapBuffers(m_hDC);

			// Set pixel format
			PIXELFORMATDESCRIPTOR pfd;
			//WGL::ZeroPixelDescriptor(pfd);
			pfd.nVersion   = 1; 
			pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
			pfd.iPixelType = (byte)(PFD_TYPE_RGBA);
			pfd.cColorBits = 32;
			pfd.cDepthBits = 32;
			pfd.iLayerType = (byte)(PFD_MAIN_PLANE);

			int nFormat = ChoosePixelFormat(m_hDC, &pfd);
			SetPixelFormat(m_hDC, nFormat, &pfd);

			// Create OpenGL Rendering Context
			m_hRC = (wglCreateContext(m_hDC));
			if (m_hRC == 0) 
				MessageBox::Show("wglCreateContext Error");
			// Assign OpenGL Rendering Context
			if (wglMakeCurrent(m_hDC, m_hRC ) == false) 
				MessageBox::Show("wglMakeCurrent Error");
			glClearColor(0.0f,0.0f,0.0f,0.0f);
			glLoadIdentity();
		
			GLfloat light_pos[]={-200,-200,200,0};
			GLfloat light_diffuse[]={0.8f,0.8f,0.8f,1.0f};
			GLfloat light_specular[]={1,1,1,1};
			GLfloat light_ambient[]={0.3f,0.3f,0.3f,1.0f};
			GLfloat light1_pos[]={200,200,-200,0};
			GLfloat light1_diffuse[]={0.0f,0.8f,0.0f,1.0f};
			GLfloat light2_pos[]={200,-200,200,0};
			GLfloat light2_diffuse[]={0.0f,0.0f,0.8f,1.0f};
			glLightfv(GL_LIGHT0,GL_POSITION,light_pos);
			glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
			glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
			glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
			glLightfv(GL_LIGHT1,GL_POSITION,light1_pos);
			glLightfv(GL_LIGHT1,GL_DIFFUSE,light1_diffuse);
			glLightfv(GL_LIGHT1,GL_SPECULAR,light1_diffuse);
			glLightfv(GL_LIGHT1,GL_AMBIENT,light_ambient);
			glLightfv(GL_LIGHT2,GL_POSITION,light2_pos);
			glLightfv(GL_LIGHT2,GL_DIFFUSE,light2_diffuse);
			glLightfv(GL_LIGHT2,GL_SPECULAR,light2_diffuse);
			glLightfv(GL_LIGHT2,GL_AMBIENT,light_ambient);
			
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHT1);
			glEnable(GL_LIGHT2);
			glEnable(GL_DEPTH_TEST);
			
			glDepthFunc(GL_LEQUAL);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			//glLoadIdentity();
		}

		void String2char(String^ in,string& out){
			const char* a = (const char*) (System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(in)).ToPointer();
			out = a;
			System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)a));
		}

		void read_obj(String^ filename,mesh* model){
			
			string file;
			String2char(filename,file);
			
			//this->test_lb->Text=gcnew String(file.c_str());
			char buff[255],out[255],o1[30],o2[30],o3[30];
			string s1,s2,s3;
			string sym ("/");
			float t1,t2,t3;
			FILE *fp;
			fopen_s(&fp,&file[0],"r");
			fgets(buff,255,fp);
			int vcount=0,fcount=0;
			while(fgets(buff,255,fp)!= NULL){
				if(sscanf_s(buff,"v %f %f %f",&t1,&t2,&t3)){
					vcount++;
				}
				if(sscanf_s(buff,"f %f %f %f",&t1,&t2,&t3)){
					fcount++;
				}
			}
			fclose(fp);
			model->setVnum(vcount);
			model->setFnum(fcount);
			
			fopen_s(&fp,&file[0],"r");
			fgets(buff,255,fp);
			vcount=0;
			fcount=0;
			while(fgets(buff,255,fp)!= NULL){
				if(sscanf_s(buff,"v %f %f %f",&t1,&t2,&t3)){
					model->setV(vcount,t1,t2,t3);
					vcount++;
				}
				if(sscanf_s(buff,"f %s %s %s",o1,30,o2,30,o3,30)){
					s1= string (o1);
					s2= string (o2);
					s3= string (o3);
					if(string::npos != s1.find(sym)){
						sscanf_s(s1.c_str(),"%f/%f",&t1,&t1);
						sscanf_s(s2.c_str(),"%f/%f",&t2,&t2);
						sscanf_s(s3.c_str(),"%f/%f",&t3,&t3);
						model->setF(fcount,(int)t1,(int)t2,(int)t3);
						fcount++;
					}else{
						sscanf_s(buff,"f %f %f %f\n",&t1,&t2,&t3);
						model->setF(fcount,(int)t1,(int)t2,(int)t3);
						fcount++;
					}
					
					//this->test_lb2->Text=gcnew String(s1.c_str());
				}
			}
			fclose(fp);
			//sprintf_s(out,255,"%f %f %f",model->getV(0).x,model->getV(0).y,model->getV(0).z);
			//this->test_lb->Text=gcnew String(out);
			model->setVN();
			model->setCenter();
			model->initPose();
			
		}
		void draw_mesh(mesh model){
			//glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(model.getTrans().x,model.getTrans().y,model.getTrans().z);
			glRotatef(model.getRotate_angle(),model.getRotate().x,model.getRotate().y,model.getRotate().z);
			glBegin(GL_TRIANGLES);
			glColor3f(1.0, 1.0, 1.0);
			for(int i=0;i<model.getFnum();i++){
				//glNormal3f(model.getN(i).x,model.getN(i).y,model.getN(i).z);
				glNormal3f(model.getVN(model.getF(i).f1-1).x,model.getVN(model.getF(i).f1-1).y,model.getVN(model.getF(i).f1-1).z);
				glVertex3f(model.getV(model.getF(i).f1-1).x,model.getV(model.getF(i).f1-1).y,model.getV(model.getF(i).f1-1).z);
				glNormal3f(model.getVN(model.getF(i).f2-1).x,model.getVN(model.getF(i).f2-1).y,model.getVN(model.getF(i).f2-1).z);
				glVertex3f(model.getV(model.getF(i).f2-1).x,model.getV(model.getF(i).f2-1).y,model.getV(model.getF(i).f2-1).z);
				glNormal3f(model.getVN(model.getF(i).f3-1).x,model.getVN(model.getF(i).f3-1).y,model.getVN(model.getF(i).f3-1).z);
				glVertex3f(model.getV(model.getF(i).f3-1).x,model.getV(model.getF(i).f3-1).y,model.getV(model.getF(i).f3-1).z);
			}
			
			glEnd();
			//glFlush();
			//SwapBuffers( m_hDC );  
			
		}
	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Panel^  showPan;
private: System::ComponentModel::IContainer^  components;
	protected: 

	
	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>

	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
		static HWND m_hWnd;
		static HDC  m_hDC;
	private: System::Windows::Forms::Button^  load_btn;
	private: System::Windows::Forms::Label^  test_lb;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  test_lb2;
	private: System::Windows::Forms::Timer^  timer1;
		 static HGLRC m_hRC;
	

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
		///
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->showPan = (gcnew System::Windows::Forms::Panel());
			this->load_btn = (gcnew System::Windows::Forms::Button());
			this->test_lb = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->test_lb2 = (gcnew System::Windows::Forms::Label());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// showPan
			// 
			this->showPan->Location = System::Drawing::Point(6, 12);
			this->showPan->Name = L"showPan";
			this->showPan->Size = System::Drawing::Size(376, 358);
			this->showPan->TabIndex = 0;
			this->showPan->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::showPan_MouseWheel);
			this->showPan->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::showPan_MouseMove);
			this->showPan->Click += gcnew System::EventHandler(this, &Form1::showPan_Click);
			this->showPan->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::showPan_KeyUp);
			this->showPan->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::showPan_MouseDown);
			this->showPan->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::showPan_MouseUp);
			this->showPan->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::showPan_KeyDown);
			// 
			// load_btn
			// 
			this->load_btn->AutoSize = true;
			this->load_btn->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->load_btn->Location = System::Drawing::Point(12, 378);
			this->load_btn->Name = L"load_btn";
			this->load_btn->Size = System::Drawing::Size(38, 22);
			this->load_btn->TabIndex = 1;
			this->load_btn->Text = L"open";
			this->load_btn->UseVisualStyleBackColor = true;
			this->load_btn->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::showPan_MouseWheel);
			this->load_btn->Click += gcnew System::EventHandler(this, &Form1::load_btn_Click);
			// 
			// test_lb
			// 
			this->test_lb->AutoSize = true;
			this->test_lb->Location = System::Drawing::Point(103, 378);
			this->test_lb->Name = L"test_lb";
			this->test_lb->Size = System::Drawing::Size(0, 12);
			this->test_lb->TabIndex = 2;
			// 
			// button1
			// 
			this->button1->AutoSize = true;
			this->button1->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->button1->Location = System::Drawing::Point(12, 407);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(51, 22);
			this->button1->TabIndex = 3;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// test_lb2
			// 
			this->test_lb2->AutoSize = true;
			this->test_lb2->Location = System::Drawing::Point(103, 412);
			this->test_lb2->Name = L"test_lb2";
			this->test_lb2->Size = System::Drawing::Size(0, 12);
			this->test_lb2->TabIndex = 4;
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 17;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->ClientSize = System::Drawing::Size(394, 477);
			this->Controls->Add(this->test_lb2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->test_lb);
			this->Controls->Add(this->load_btn);
			this->Controls->Add(this->showPan);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &Form1::Form1_FormClosed);
			this->Resize += gcnew System::EventHandler(this, &Form1::Form1_Resize);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
	
	private: System::Void load_btn_Click(System::Object^  sender, System::EventArgs^  e) {
			 OpenFileDialog^ openfiledialog1= gcnew OpenFileDialog;
			 openfiledialog1->Filter="obj files (*.obj)|*.obj";
			 if(openfiledialog1->ShowDialog()==System::Windows::Forms::DialogResult::OK){
				 Eyex=0;Eyey=0;Eyez=100;
				 read_obj(openfiledialog1->FileName,&Model1);
				 Model1.setTrans(-Model1.getCenter().x,-Model1.getCenter().y,-Model1.getCenter().z);
				 this->showPan->Focus();
				 Model1_load=true;
				 //draw_mesh(Model);
			 }
			 
		 }

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				/*//void *WindowHandle1;
				char filename[]="ma.jpg";
				IplImage *Im1;
				Im1=cvLoadImage(filename,1);
				//cvNamedWindow("Show",0);
				//cvResizeWindow("Show",360,429);
				//WindowHandle1=cvGetWindowHandle("Show");
				cvShowImage("Show",Im1);
				cvWaitKey(0);
				cvDestroyWindow("Show");
				cvReleaseImage(&Im1);*/
				OpenFileDialog^ openfiledialog1= gcnew OpenFileDialog;
				openfiledialog1->Filter="obj files (*.obj)|*.obj";
				if(openfiledialog1->ShowDialog()==System::Windows::Forms::DialogResult::OK){
					read_obj(openfiledialog1->FileName,&Model2);
					//Model2.setTrans(-Model1.getCenter().x,-Model1.getCenter().y,-Model1.getCenter().z);
					this->showPan->Focus();
					Model2_load=true;
				 //draw_mesh(Model);
				}
				
		 }
private: System::Void showPan_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				click=true;
				Mousex=(int)e->X;
				Mousey=(int)e->Y;
			 }
private: System::Void showPan_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if(click==true){
				 this->test_lb->Text=System::Convert::ToString(e->Location::get());
			 if((int)e->X-Mousex>0){
				//Eyex-=(double)((int)e->X-Mousex)/100;
				Eyex-=sqrt(Eyex*Eyex+Eyey*Eyey+Eyez*Eyez)/50;
				Mousex=(int)e->X;
				
			 }else if((int)e->X-Mousex<0){
				Eyex+=sqrt(Eyex*Eyex+Eyey*Eyey+Eyez*Eyez)/50;
				Mousex=(int)e->X;
			 }
			 if((int)e->Y-Mousey>0){
				Eyey+=sqrt(Eyex*Eyex+Eyey*Eyey+Eyez*Eyez)/50;
				Mousey=(int)e->Y;
			 }else if((int)e->Y-Mousey<0){
				Eyey-=sqrt(Eyex*Eyex+Eyey*Eyey+Eyez*Eyez)/50;
				Mousey=(int)e->Y;
			 }
			 //char out[255];
			 //sprintf_s(out,255,"%lf %lf",(double)Eyex,(double)Eyey);
			 //this->test_lb->Text=gcnew String(out);
			 //draw_mesh(Model);
			 }
		 }
private: System::Void showPan_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 click=false;
		 }
private: System::Void showPan_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			 //this->test_lb->Text=System::Convert::ToString(e->KeyCode);
			 if(e->KeyCode==Keys::W || e->KeyCode==Keys::S){
				 if(key==false){
					double length;
					movex=Focusx-Eyex;
					movey=Focusy-Eyey;
					movez=Focusz-Eyez;
					length=pow(movex*movex+movey*movey+movez*movez,0.5);
					movex=movex/length*move_step;
					movey=movey/length*move_step;
					movez=movez/length*move_step;
				}
				if(e->KeyCode==Keys::W){
					Eyex+=movex;
					Eyey+=movey;
					Eyez+=movez;	
				}else{
					Eyex-=movex;
					Eyey-=movey;
					Eyez-=movez;	
				}
				key=true;
			 }

			 if(e->KeyCode==Keys::A || e->KeyCode==Keys::D){
				 rotx=Eyex-Focusx;
				 roty=Eyey-Focusy;
				 rotz=Eyez-Focusz;
				 if(e->KeyCode==Keys::D){
					Eyex =rotx*cos(PI*rot_angle/180)+rotz*sin(PI*rot_angle/180)+Focusx;
					Eyey =roty+Focusy;
					Eyez =-rotx*sin(PI*rot_angle/180)+rotz*cos(PI*rot_angle/180)+Focusz;
				 }else{
					Eyex =rotx*cos(-PI*rot_angle/180)+rotz*sin(-PI*rot_angle/180)+Focusx;
					Eyey =roty+Focusy;
					Eyez =-rotx*sin(-PI*rot_angle/180)+rotz*cos(-PI*rot_angle/180)+Focusz;
				 }
				 key=true;
			 }
			 
			 if(e->KeyCode==Keys::Q || e->KeyCode==Keys::E){
				 rotx=Eyex-Focusx;
				 roty=Eyey-Focusy;
				 rotz=Eyez-Focusz;
				 if(e->KeyCode==Keys::Q){
					Eyex =rotx+Focusx;
					Eyey =roty*cos(PI*rot_angle/180)-rotz*sin(PI*rot_angle/180)+Focusy;
					Eyez =roty*sin(PI*rot_angle/180)+rotz*cos(PI*rot_angle/180)+Focusz;
				 }else{
					Eyex =rotx+Focusx;
					Eyey =roty*cos(-PI*rot_angle/180)-rotz*sin(-PI*rot_angle/180)+Focusy;
					Eyez =roty*sin(-PI*rot_angle/180)+rotz*cos(-PI*rot_angle/180)+Focusz;
				 }
				 key=true;
			 }
			 if(e->KeyCode==Keys::NumPad4){
				 Model2.setTrans(Model2.getTrans().x-5,Model2.getTrans().y,Model2.getTrans().z);
				 this->test_lb2->Text="pressed";
			 }
			 
		 }
private: System::Void showPan_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			 key=false;
			 
		 }
private: System::Void showPan_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 //this->test_lb->Text=System::Convert::ToString(e->Delta);
			 if(e->Delta>0)
				 Eyez-=(ViewFar-ViewNear)/1000;
			 else
				 Eyez+=(ViewFar-ViewNear)/1000;
			 //draw_mesh(Model);
		 }

private: System::Void Form1_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 Model1.kill();
			 Model2.kill();
		 }


private: System::Void showPan_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->showPan->Focus();
		 }
private: System::Void Form1_Resize(System::Object^  sender, System::EventArgs^  e) {
			 this->showPan->Width=(int)(this->Width-40);
			 this->showPan->Height=(int)(this->Height-150);
			 glViewport(0,0,this->showPan->Width,this->showPan->Height);
		 }
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
			 if(Model1_load&&Model2_load){
				wglMakeCurrent(m_hDC, m_hRC);
				glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				//glOrtho(ViewLeft,ViewRight,ViewBottom,ViewTop,ViewNear,ViewFar);
				gluPerspective(45,(double)(this->showPan->Width/this->showPan->Height),0.1,ViewFar);
				gluLookAt(Eyex,Eyey,Eyez,Focusx,Focusy,Focusz,0,1,0);
				glMatrixMode(GL_MODELVIEW);
				draw_mesh(Model1);
				draw_mesh(Model2);
				glFlush();
				SwapBuffers( m_hDC );  
			 }else if(Model1_load){
				wglMakeCurrent(m_hDC, m_hRC);
				glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				//glOrtho(ViewLeft,ViewRight,ViewBottom,ViewTop,ViewNear,ViewFar);
				gluPerspective(45,(double)(this->showPan->Width/this->showPan->Height),0.1,ViewFar);
				gluLookAt(Eyex,Eyey,Eyez,Focusx,Focusy,Focusz,0,1,0);
				glMatrixMode(GL_MODELVIEW);
				draw_mesh(Model1);
				glFlush();
				SwapBuffers( m_hDC );  
			 }else if(Model2_load){
				wglMakeCurrent(m_hDC, m_hRC);
				glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				//glOrtho(ViewLeft,ViewRight,ViewBottom,ViewTop,ViewNear,ViewFar);
				gluPerspective(45,(double)(this->showPan->Width/this->showPan->Height),0.1,ViewFar);
				gluLookAt(Eyex,Eyey,Eyez,Focusx,Focusy,Focusz,0,1,0);
				glMatrixMode(GL_MODELVIEW);
				draw_mesh(Model2);
				glFlush();
				SwapBuffers( m_hDC );  
			}
			 char out[255];
			 sprintf_s(out,255,"%lf %lf %lf",(double)Eyex,(double)Eyey,(double)Eyez);
			 this->test_lb->Text=gcnew String(out);
			 sprintf_s(out,255,"%lf %lf %lf",(double)Focusx,(double)Focusy,(double)Focusz);
			 this->test_lb2->Text=gcnew String(out);
		 }
};
}

