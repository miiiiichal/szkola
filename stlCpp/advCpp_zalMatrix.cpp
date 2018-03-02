//============================================================================
// Name        : advCpp_zalMatrix.cpp
// Author      : mj
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>


template<std::size_t M ,std::size_t N, typename T>
class MyMatrix{

public:
	std::vector<std::vector<T>> matrix;
	//def_ctor
	MyMatrix(){
		std::cout<<"default ctor"<<std::endl;
		matrix.reserve(M);
		for(auto &n:matrix)
			n.reserve(N);
	};
	//cpy_ctor
	MyMatrix(const MyMatrix<M,N,T> &other){
		std::cout<<"copy ctor"<<std::endl;
		matrix = other.matrix;
	};
	//

	template<std::size_t M1, std::size_t N1>
	MyMatrix(const MyMatrix<M1,N1,T> &other){
		std::cout<<"specialized copy ctor"<<std::endl;
		if(matrix.size() != M1){
			matrix.resize(other.matrix.size());
			for(size_t i=0;i<matrix.size();++i){
				if(matrix[i].size()!=N1)
					matrix[i].resize(other.matrix[i].size());
				std::copy(other.matrix[i].begin(),other.matrix[i].end(),matrix[i].begin());
			}
		}
	};

	MyMatrix(const MyMatrix && _matrix) :matrix(std::move(_matrix.matrix)) {};

	MyMatrix& operator+(const MyMatrix<M,N,T>& rhs){

		for(size_t m=0; m<matrix.size();++m){
			for(size_t n=0; n<matrix[m].size();++n){
				matrix[m][n]+=rhs.matrix[m][n];
			}
		}
	/*
	 * //WHY TYPENAME ???
		typename std::vector<std::vector<T>>::iterator it_r;
		typename std::vector<T>::iterator it_c;
	  for(it_r=matrix.begin();it_r!=matrix.end();it_r++){
			for(it_c=(*it_r).begin();it_c!=(*it_r).end();it_c++){

			}
		}
	*/
		return *this;
	};

	MyMatrix& operator+(const T s){
		for(auto &r:matrix)
			for(auto &c:r)
				c+=s;
		return *this;

	};

	MyMatrix& operator*(const T s){
		for(auto &r:matrix)
			for(auto &c:r)
				c*=s;
		return *this;
	};

	void fillMatrixWithRandomVal(){
		/*	for(size_t it0 = 0; it0<matrix.capacity();it0++){
				std::vector<T> v_;
				v_.reserve(N);
				for(size_t it = 0; it<v_.capacity();it++){
					//v_[it]=val;
					T val = random()%100;
					v_.push_back(val);
					//std::cout<< v_[it];
					if(it<v_.capacity()-1)
						//std::cout<<",";
				}
				matrix.push_back(v_);
				//std::cout<<std::endl;
			}
		 */
		std::generate_n(std::back_inserter(matrix),M,[](){
			std::vector<T> v_;
			std::generate_n(std::back_inserter(v_),N,[](){return (rand()%5);});
			return(v_);
		});
		}
	void initMatrix(){
			std::generate_n(std::back_inserter(matrix),M,[](){
				std::vector<T> v_;
				T x;
				std::generate_n(std::back_inserter(v_),N,[x](){return x;});
				return(v_);
			});
	}
	void initMatrix(T x){
				std::generate_n(std::back_inserter(matrix),M,[x](){
					std::vector<T> v_;

					std::generate_n(std::back_inserter(v_),N,[x](){return x;});
					return(v_);
				});
		}

	void printMatrix(){
			/*for(size_t i=0;i<matrix.size();i++){
				size_t j=0;
				for(auto &m2:matrix[i]){
					std::cout<<m2;
					if(j<matrix[i].size()-1)
						std::cout<<" , ";
					j++;
				}
				std::cout<<std::endl;
			}
			*/
			std::for_each(matrix.begin(), matrix.end(), [](std::vector<T> &m){
				std::for_each(m.begin(),m.end(),[](auto &n){
						std::cout<<n<<" ";
					});
					std::cout<<std::endl;
			});
		}

	template<std::size_t M1, std::size_t N1, typename T1>
	friend std::ostream& operator<<(std::ostream&, const MyMatrix<M,N,T>&);
};

template<std::size_t M, std::size_t N, typename T>
	std::ostream& operator<<(std::ostream& os, const MyMatrix<M,N,T> &mat){
		for(auto &a:mat.matrix){
			for(auto &w:a){
				os<< w;
				os<<" ";
			}
			os<<std::endl;
		}
		return os;2
	};

template<std::size_t A,std::size_t B,std::size_t C, typename T>
MyMatrix<A,C,T>& operator*(const MyMatrix<A,B,T> &first, const MyMatrix<B,C,T> second) {

	auto f = first.matrix;
	auto s = second.matrix;
	MyMatrix<A,C,T> *tmp =new MyMatrix<A,C,T>();
	tmp->initMatrix();

	for(size_t i=0;i<A;++i)
		for(size_t j=0;j<C;++j)
			for(size_t k=0;k<B;++k){
				tmp->matrix[i][j]+= f[i][k]*s[k][j];
			}

	//tmp->printMatrix();


	return *tmp;
}


int main() {
	std::cout << "matrix zal AdvCpp" << std::endl; // prints

	const size_t m =3, n=2;
	MyMatrix<m,n,int> m1;
	std::cout << "----------------"<<std::endl;
	m1.fillMatrixWithRandomVal();
	m1.printMatrix();
/*
	MyMatrix<m,n,int> m2;
	m2.fillMatrixWithRandomVal();
	std::cout << "----------------"<<std::endl;
	std::cout << m2;
	std::cout << "----------------"<<std::endl;
	auto m3(m1+m2);
	std::cout<<"m3"<<std::endl << m3;
	std::cout << "----------------"<<std::endl;
*/
	MyMatrix<2,4,int> m4;
	m4.fillMatrixWithRandomVal();
	std::cout<<m4;


	MyMatrix<m,n,int> *m5;
	m5 =new MyMatrix<m,n,int>(m1*m4);
	m5->printMatrix();

	MyMatrix<m,n,int> m6(std::move(m1));

	m1.matrix[2][1]=100;
	m6.matrix[2][0]=200;


	return 0;
}
