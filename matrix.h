#ifndef MATRIX001_H


//	�X�v���C���p�̍ŏ����̍s��N���X


#define MATRIX001_H 1

// �s��̏��

enum MATRIX_TYPE{ 	undecided=0,upper_triangular,lowwer__triangular };

// 1��̍s��N���X

class MATRIX_OWN_COLUMN{
	double* d;	//	�s��f�[�^�[
	double k;	//	�s��̕�������я搔�������@�Ⴆ�΃X�J���[�{�����ꍇ�͂��̌W���̂ݏ悸��B
	int rmax;	//	�s��@�s��
	int max;	//	�m�ۂ���Ă���z��̃T�C�Y
	MATRIX_TYPE flag;	//	�s��^�C�v
public:
	MATRIX_OWN_COLUMN(){
		d=0;
		rmax=max=0;
	}
	MATRIX_OWN_COLUMN(int ar){
		d=0;
		rmax=max=0;
		alloc(ar);
	}
	~MATRIX_OWN_COLUMN(){
		delete [] d;
	}
	void alloc(int ar){
		if(max<ar){	//	�z��T�C�Y���s�����Ă���ꍇ�͍Ċm�ۂ���
			max=rmax=ar;
			if(d)
				delete [] d;
			d=new double[rmax];
		}else{
			rmax=ar;
		}
		flag=undecided;
		k=1;
	}
	void rswap(int a,int b){	//	�s�̌���
		double t=d[a];
		d[a]=d[b];
		d[b]=t;
		k=-k;
	}
	void set(double* ad){	//	�s��ւ̒萔�̑��
		for(int n=0;n<rmax;n++)
			d[n]=ad[n];
	}
	void put(void){	//	�s���W���o�͂֕\��
		for(int rn=0;rn<rmax;rn++){
			_tprintf(TEXT("%i %g\n"),rn,d[rn]);
		}
	}
	double& operator[](int n){
		return d[n];
	}
	friend class MATRIX;
};

//	�����s��N���X


class MATRIX{
	double* d;	//	�s��f�[�^�[
	int cmax,rmax;	//	�s��@�� �s��
	double k;	//	�s��̕�������я搔�������@�Ⴆ�΃X�J���[�{�����ꍇ�͂��̌W���̂ݏ悸��Bdet
	int max;	//	�m�ۂ���Ă���z��̃T�C�Y
	MATRIX_TYPE flag;	//	�s��^�C�v
public:
	MATRIX(){
		d=0;
		cmax=rmax=0;
		max=0;
		k=1;
	}
	MATRIX(int ar,int ac){
		d=0;
		cmax=rmax=0;
		max=0;
		k=1;
		alloc(ar,ac);
	}
	~MATRIX(){
		delete [] d;
	}
	void alloc(int ar,int ac){
		cmax=ac;
		rmax=ar;
		if(max<ar*ac){	//	�z��T�C�Y���s������ꍇ�͔z����Ċm�ۂ���
			if(d){
				delete [] d;
			}
			max=cmax*rmax;
			d=new double[max];
		}
		flag=undecided;
		k=1;
	}
	double* operator[](int n){
		return d+n*cmax;
	}
	void rswap(int a,int b){	//	�s�̌���
		for(int i=0;i<cmax;i++){
			double t=(*this)[a][i];
			(*this)[a][i]=(*this)[b][i];
			(*this)[b][i]=t;
		}
		k=-k;	//	�����𔽓]����
	}
	void set(double* ad){	//	�s��ւ̒萔�̑��
		for(int n=0;n<cmax*rmax;n++)
			d[n]=ad[n];
	}
	void put(void){	//	�s���W���o�͂֕\��
		for(int rn=0;rn<rmax;rn++){
			for(int cn=0;cn<cmax;cn++){
				_tprintf(TEXT("%g "),(*this)[rn][cn]);
			}
			_puttc(_T('\n'),stdout);
		}
	}
	void Triangular(bool pivoting=true){ // ��O�p�s��ɕϊ�(�K�E�X�̑O�i�����@) �����Ńs�|�b�g�I���̗L����I��
		int i,j,k;
		double uk1;
		for(k=0;k<rmax-1;k++){
			double m=0;
			double u;
			int n=k;
			if(pivoting){	//	�s�|�b�g�I�����s��
				for(i=k;i<rmax;i++){	//	��Βl���ő�̍s�̌���
					u=fabs( (*this)[i][k] );
					if(m!=0 && m<u){
						m=u;
						n=i;
					}
				}
				if(n!=k)
					rswap(n,k);	//	��Βl���ő�̍s�Ɠ���ւ�
			}
			for(i=k+1;i<rmax;i++){
				uk1 = (*this)[i][k] / (*this)[k][k];	//	  ui1=ai1/a11
				(*this)[i][k]=0;		//	��O�p�s��ɂ���ƎQ�Ƃ���Ȃ��̂ň�ʓI�ɂ͕s�v
				for(j=k+1;j<cmax;j++){
					 (*this)[i][j] = (*this)[i][j] - uk1 * (*this)[k][j];	 //aji = aji - ui1 * a1j
				}
			}
		}
		flag=upper_triangular;	//	�s��^�C�v��ݒ�
	}
	void forward_sub(MATRIX_OWN_COLUMN& b,bool pivoting=true){ // ��O�p�s��ɕϊ�(�K�E�X�̑O�i�����@)  �����Ńs�|�b�g�I���̗L����I��
		int i,j,k;
		double uk1;
		for(k=0;k<rmax-1;k++){
			double m=0;
			double u=0;
			int n=k;
			if(pivoting){
				for(i=k;i<rmax;i++){	//	��Βl���ő�̍s�̌���
					u=fabs( (*this)[i][k] );
					if(m<u){
						m=u;
						n=i;
					}
				}
				if(m!=0 && n!=k){
					rswap(n,k);	//	��Βl���ő�̍s�Ɠ���ւ����������ׂĂ�0�̎��͓���ւ��Ȃ�
					b.rswap(n,k);
				}
			}
			for(i=k+1;i<rmax;i++){
				uk1 = (*this)[i][k] / (*this)[k][k];	//	  ui1=ai1/a11
 				b[i] = b[i] - uk1 * b[k];
 				(*this)[i][k]=0;		//	��O�p�s��ɂ���ƎQ�Ƃ���Ȃ��̂ň�ʓI�ɂ͕s�v

				for(j=k+1;j<cmax;j++){
					 (*this)[i][j] = (*this)[i][j] - uk1 * (*this)[k][j];	 //aji = aji - ui1 * a1j
				}
			}
		}
		flag=upper_triangular;	//	�s��^�C�v��ݒ�
	}
	void forward_sub(MATRIX_OWN_COLUMN* b,int nmax, bool pivoting=true){ // ��O�p�s��ɕϊ�(�K�E�X�̑O�i�����@) �s�|�b�g�I��L��
		int i,j,k;
		double uk1;
		for(k=0;k<rmax-1;k++){
			double m=0;
			double u=0;
			int n=k;
			if(pivoting){
				for(i=k;i<rmax;i++){	//	��Βl���ő�̍s�̌���
					u=fabs( (*this)[i][k] );
					if(m<u){
						m=u;
						n=i;
					}
				}
				if(m!=0 && n!=k){
					rswap(n,k);	//	��Βl���ő�̍s�Ɠ���ւ����������ׂĂ�0�̎��͓���ւ��Ȃ�
					for(int o=0;o<nmax;o++){
						b[o].rswap(n,k);
					}
				}
			}

			for(i=k+1;i<rmax;i++){
				uk1 = (*this)[i][k] / (*this)[k][k];	//	  ui1=ai1/a11
				for(int o=0;o<nmax;o++){
					MATRIX_OWN_COLUMN& t=b[o];
					t[i] = t[i] - uk1 * t[k];
				}
 				(*this)[i][k]=0;		//	��O�p�s��ɂ���ƎQ�Ƃ���Ȃ��̂ň�ʓI�ɂ͕s�v

				for(j=k+1;j<cmax;j++){
					 (*this)[i][j] = (*this)[i][j] - uk1 * (*this)[k][j];	 //aji = aji - ui1 * a1j
				}
			}
		}
		flag=upper_triangular;	//	�s��^�C�v��ݒ�
	}
	void backward_sub(MATRIX_OWN_COLUMN& b,MATRIX_OWN_COLUMN& x){	//	��ޑ���@ ���炩���ߏ�O�p�s��ɕϊ����Ă����K�v������B
		int i,j;
		for(i=rmax-1;0<=i;i--){
			double c=b[i]*b.k;
			for(j=i+1;j<rmax;j++){
				c -= (*this)[i][j] * x[j];
			}
			x[i] = c / (*this)[i][i];
		}
	}
	void backward_sub(MATRIX_OWN_COLUMN* b,MATRIX_OWN_COLUMN* x,int nmax){	//	��ޑ���@ ���炩���ߏ�O�p�s��ɕϊ����Ă����K�v������B
		int i,j;
		for(i=rmax-1;0<=i;i--){
			for(int n=0;n<nmax;n++){
				double c=b[n][i]*b[n].k;
				for(j=i+1;j<rmax;j++){
					c -= (*this)[i][j] * x[n][j];
				}
				x[n][i] = c / (*this)[i][i];
			}
		}
	}
	double det(void){	//	�s��̒l���Z�o
		double a=k;
		if(flag==upper_triangular){	//	��O�p�s��
			for(int i=0;i<cmax;i++)
				a *= (*this)[i][i];
			return a;
		}else{	// ��O�p�s��ɕϊ����Ă���l�����߂�
			MATRIX x(rmax,cmax);
			x=(*this);
			x.Triangular();
			return x.det();
		}
	}
	void zero(void){	//	�s��̏�����
		ZeroMemory((void*)d,sizeof(d[0])*rmax*cmax);
	}
};

#endif