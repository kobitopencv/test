#ifndef MATRIX001_H


//	スプライン用の最小限の行列クラス


#define MATRIX001_H 1

// 行列の状態

enum MATRIX_TYPE{ 	undecided=0,upper_triangular,lowwer__triangular };

// 1列の行列クラス

class MATRIX_OWN_COLUMN{
	double* d;	//	行列データー
	double k;	//	行列の符号および乗数を示す　例えばスカラー倍した場合はこの係数のみ乗ずる。
	int rmax;	//	行列　行数
	int max;	//	確保されている配列のサイズ
	MATRIX_TYPE flag;	//	行列タイプ
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
		if(max<ar){	//	配列サイズが不足している場合は再確保する
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
	void rswap(int a,int b){	//	行の交換
		double t=d[a];
		d[a]=d[b];
		d[b]=t;
		k=-k;
	}
	void set(double* ad){	//	行列への定数の代入
		for(int n=0;n<rmax;n++)
			d[n]=ad[n];
	}
	void put(void){	//	行列を標準出力へ表示
		for(int rn=0;rn<rmax;rn++){
			_tprintf(TEXT("%i %g\n"),rn,d[rn]);
		}
	}
	double& operator[](int n){
		return d[n];
	}
	friend class MATRIX;
};

//	正方行列クラス


class MATRIX{
	double* d;	//	行列データー
	int cmax,rmax;	//	行列　列数 行数
	double k;	//	行列の符号および乗数を示す　例えばスカラー倍した場合はこの係数のみ乗ずる。det
	int max;	//	確保されている配列のサイズ
	MATRIX_TYPE flag;	//	行列タイプ
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
		if(max<ar*ac){	//	配列サイズが不足する場合は配列を再確保する
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
	void rswap(int a,int b){	//	行の交換
		for(int i=0;i<cmax;i++){
			double t=(*this)[a][i];
			(*this)[a][i]=(*this)[b][i];
			(*this)[b][i]=t;
		}
		k=-k;	//	符号を反転する
	}
	void set(double* ad){	//	行列への定数の代入
		for(int n=0;n<cmax*rmax;n++)
			d[n]=ad[n];
	}
	void put(void){	//	行列を標準出力へ表示
		for(int rn=0;rn<rmax;rn++){
			for(int cn=0;cn<cmax;cn++){
				_tprintf(TEXT("%g "),(*this)[rn][cn]);
			}
			_puttc(_T('\n'),stdout);
		}
	}
	void Triangular(bool pivoting=true){ // 上三角行列に変換(ガウスの前進消去法) 引数でピポット選択の有無を選択
		int i,j,k;
		double uk1;
		for(k=0;k<rmax-1;k++){
			double m=0;
			double u;
			int n=k;
			if(pivoting){	//	ピポット選択を行う
				for(i=k;i<rmax;i++){	//	絶対値が最大の行の検索
					u=fabs( (*this)[i][k] );
					if(m!=0 && m<u){
						m=u;
						n=i;
					}
				}
				if(n!=k)
					rswap(n,k);	//	絶対値が最大の行と入れ替え
			}
			for(i=k+1;i<rmax;i++){
				uk1 = (*this)[i][k] / (*this)[k][k];	//	  ui1=ai1/a11
				(*this)[i][k]=0;		//	上三角行列にすると参照されないので一般的には不要
				for(j=k+1;j<cmax;j++){
					 (*this)[i][j] = (*this)[i][j] - uk1 * (*this)[k][j];	 //aji = aji - ui1 * a1j
				}
			}
		}
		flag=upper_triangular;	//	行列タイプを設定
	}
	void forward_sub(MATRIX_OWN_COLUMN& b,bool pivoting=true){ // 上三角行列に変換(ガウスの前進消去法)  引数でピポット選択の有無を選択
		int i,j,k;
		double uk1;
		for(k=0;k<rmax-1;k++){
			double m=0;
			double u=0;
			int n=k;
			if(pivoting){
				for(i=k;i<rmax;i++){	//	絶対値が最大の行の検索
					u=fabs( (*this)[i][k] );
					if(m<u){
						m=u;
						n=i;
					}
				}
				if(m!=0 && n!=k){
					rswap(n,k);	//	絶対値が最大の行と入れ替えただしすべてが0の時は入れ替えない
					b.rswap(n,k);
				}
			}
			for(i=k+1;i<rmax;i++){
				uk1 = (*this)[i][k] / (*this)[k][k];	//	  ui1=ai1/a11
 				b[i] = b[i] - uk1 * b[k];
 				(*this)[i][k]=0;		//	上三角行列にすると参照されないので一般的には不要

				for(j=k+1;j<cmax;j++){
					 (*this)[i][j] = (*this)[i][j] - uk1 * (*this)[k][j];	 //aji = aji - ui1 * a1j
				}
			}
		}
		flag=upper_triangular;	//	行列タイプを設定
	}
	void forward_sub(MATRIX_OWN_COLUMN* b,int nmax, bool pivoting=true){ // 上三角行列に変換(ガウスの前進消去法) ピポット選択有り
		int i,j,k;
		double uk1;
		for(k=0;k<rmax-1;k++){
			double m=0;
			double u=0;
			int n=k;
			if(pivoting){
				for(i=k;i<rmax;i++){	//	絶対値が最大の行の検索
					u=fabs( (*this)[i][k] );
					if(m<u){
						m=u;
						n=i;
					}
				}
				if(m!=0 && n!=k){
					rswap(n,k);	//	絶対値が最大の行と入れ替えただしすべてが0の時は入れ替えない
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
 				(*this)[i][k]=0;		//	上三角行列にすると参照されないので一般的には不要

				for(j=k+1;j<cmax;j++){
					 (*this)[i][j] = (*this)[i][j] - uk1 * (*this)[k][j];	 //aji = aji - ui1 * a1j
				}
			}
		}
		flag=upper_triangular;	//	行列タイプを設定
	}
	void backward_sub(MATRIX_OWN_COLUMN& b,MATRIX_OWN_COLUMN& x){	//	後退代入法 あらかじめ上三角行列に変換しておく必要がある。
		int i,j;
		for(i=rmax-1;0<=i;i--){
			double c=b[i]*b.k;
			for(j=i+1;j<rmax;j++){
				c -= (*this)[i][j] * x[j];
			}
			x[i] = c / (*this)[i][i];
		}
	}
	void backward_sub(MATRIX_OWN_COLUMN* b,MATRIX_OWN_COLUMN* x,int nmax){	//	後退代入法 あらかじめ上三角行列に変換しておく必要がある。
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
	double det(void){	//	行列の値を算出
		double a=k;
		if(flag==upper_triangular){	//	上三角行列
			for(int i=0;i<cmax;i++)
				a *= (*this)[i][i];
			return a;
		}else{	// 上三角行列に変換してから値を求める
			MATRIX x(rmax,cmax);
			x=(*this);
			x.Triangular();
			return x.det();
		}
	}
	void zero(void){	//	行列の初期化
		ZeroMemory((void*)d,sizeof(d[0])*rmax*cmax);
	}
};

#endif