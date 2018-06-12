//
// Created by Chingkai Chou on 5/25/18.
//

#ifndef ARTPDE_POSITION_VECTOR_HPP
#define ARTPDE_POSITION_VECTOR_HPP

#include <iostream>
#include <array>
#include <memory>
#include <algorithm>
#include <assert.h>

namespace art_pde{ namespace position_vector{

    // -------- PointData <Start> -----------
    template <size_t Dimension>
        class PointData{
        public:
            typedef std::array<double,Dimension> ArrayType;
            typedef std::shared_ptr<ArrayType> PtrArrayType;

            PointData<Dimension>& operator=(const PointData<Dimension>& other);

            template <size_t Dimension_>
            friend std::ostream &operator<<(std::ostream &os, const PointData<Dimension_> &point_data);

        protected:
            PointData(){ this->newData(); }
            PointData(const std::initializer_list<double> &v){ this->newData(); this->addDataByList(v);}
            void newData();
            void addDataByList(const std::initializer_list<double> &v);
            const double &getDataById(const size_t &id) const;
            void setDataById(const size_t &id, const double &value);
            void setDataById(const size_t &id, double &value);

            PtrArrayType data{nullptr}; //real data storage
        };
//#include "./src/position_vector_pointdata_impl.cpp"

		template <size_t Dimension>
		void PointData<Dimension>::newData(){
			this->data = nullptr;
			this->data = std::make_shared<ArrayType>();
			//std::cout << "PointData new data" << std::endl;
		};

		template <size_t Dimension>
		void PointData<Dimension>::addDataByList(const std::initializer_list<double> &v){
			assert(v.size() <= Dimension);
			std::copy(v.begin(), v.end(), data->begin());
		};

		template <size_t Dimension>
		PointData<Dimension>& PointData<Dimension>::operator=(const PointData<Dimension>& other)
		{
			//std::cout << "copy" << std::endl;
			if (&other == this)
				return *this;
			if (this->data == nullptr)
				this->newData();
			std::copy(other.data->begin(), other.data->end(), this->data->begin());
			return *this;
		};

		template <size_t Dimension>
		std::ostream &operator<<(std::ostream &os, const PointData<Dimension> &point_data) {
			os << "[ ";
			for (size_t i = 0; i < point_data.data->size() - 1; ++i) {
				os << point_data.data->at(i) << " ";
			}
			os << point_data.data->at(point_data.data->size() - 1);
			os << " ]";
			return os;
		};

		template <size_t Dimension>
		const double &PointData<Dimension>::getDataById(const size_t &id) const{
			assert(id < Dimension);
			return this->data->at(id);
		};

		template <size_t Dimension>
		void PointData<Dimension>::setDataById(const size_t &id, const double &value){
			assert(id < Dimension);
			this->data->at(id) = value;
		}

		template <size_t Dimension>
		void PointData<Dimension>::setDataById(const size_t &id, double &value){
			assert(id < Dimension);
			this->data->at(id) = value;
		}

// -------- PointData <End> -----------

// -------- CartesianAPI <Start> -----------
// #include "./src/position_vector_cartesian_impl.cpp"

		template <size_t Dimension, bool Authority> class CartesianAPI;
		template <size_t Dimension, bool Authority, typename T> struct CartesianAPI_FuncHelper;
		template <size_t Dimension, bool Authority, typename T> struct CartesianAPI_FuncWrap;

		// -------- CartesianAPI <Start> -----------
		template <size_t Dimension>
		struct CartesianAPI<Dimension, true> :
			public CartesianAPI_FuncWrap<Dimension, true, PointData<Dimension>>{

			CartesianAPI<Dimension, true> &operator=(const CartesianAPI<Dimension, true> &other) {
				PointData<Dimension>::operator=(other);
				return *this;
			}

		};

		template <size_t Dimension>
		struct CartesianAPI<Dimension, false> :
			public CartesianAPI_FuncWrap<Dimension, false, PointData<Dimension>>{

			CartesianAPI<Dimension, false>& operator=(const CartesianAPI<Dimension, false>& other)
			{
				PointData<Dimension>::operator=(other);
				return *this;
			}

			const double &getDataById(const size_t id) const{
				return PointData<Dimension>::getDataById(id);
			}
		};

		// -------- CartesianAPI <End> -----------


		// -------- CartesianAPI_FuncHelper <Start> -----------
		template<typename T> struct CartesianAPI_FuncHelper<1, false, T> :
			public virtual T{
			const double &getX() const { return this->data->at(0); }
		};
		template<typename T> struct CartesianAPI_FuncHelper<1, true, T> :
			public virtual T{
			void setX(const double &x){ this->data->at(0) = x; }
			void setX(double &x){ this->data->at(0) = x; }
		};
		template<typename T> struct CartesianAPI_FuncHelper<2, false, T> :
			public virtual T{
			const double &getY() const { return this->data->at(1); }
		};
		template<typename T> struct CartesianAPI_FuncHelper<2, true, T> :
			public virtual T{
			void setY(const double &y){ this->data->at(1) = y; }
			void setY(double &y){ this->data->at(1) = y; }
		};
		template<typename T> struct CartesianAPI_FuncHelper<3, false, T> :
			public virtual T{
			const double &getZ() const { return this->data->at(2); }
		};
		template<typename T> struct CartesianAPI_FuncHelper<3, true, T> :
			public virtual T{
			void setZ(const double &z){ this->data->at(2) = z; }
			void setZ(double &z){ this->data->at(2) = z; }
		};
		// -------- CartesianAPI_FuncHelper <End> -----------

		// -------- CartesianAPI_FuncWrap <Start> -----------
		template <typename T>
		struct CartesianAPI_FuncWrap<1, false, T> :
			public CartesianAPI_FuncHelper<1, false, T>{};

		template <typename T>
		struct CartesianAPI_FuncWrap<1, true, T> :
			public CartesianAPI_FuncHelper<1, false, T>,
			public CartesianAPI_FuncHelper<1, true, T>{};

		template <typename T>
		struct CartesianAPI_FuncWrap<2, false, T> :
			public CartesianAPI_FuncHelper<1, false, T>,
			public CartesianAPI_FuncHelper<2, false, T>{};

		template <typename T>
		struct CartesianAPI_FuncWrap<2, true, T> :
			public CartesianAPI_FuncHelper<1, false, T>,
			public CartesianAPI_FuncHelper<2, false, T>,
			public CartesianAPI_FuncHelper<1, true, T>,
			public CartesianAPI_FuncHelper<2, true, T>{};

		template <typename T>
		struct CartesianAPI_FuncWrap<3, false, T> :
			public CartesianAPI_FuncHelper<1, false, T>,
			public CartesianAPI_FuncHelper<2, false, T>,
			public CartesianAPI_FuncHelper<3, false, T>{};

		template <typename T>
		struct CartesianAPI_FuncWrap<3, true, T> :
			public CartesianAPI_FuncHelper<1, false, T>,
			public CartesianAPI_FuncHelper<2, false, T>,
			public CartesianAPI_FuncHelper<3, false, T>,
			public CartesianAPI_FuncHelper<1, true, T>,
			public CartesianAPI_FuncHelper<2, true, T>,
			public CartesianAPI_FuncHelper<3, true, T>{};
		// -------- CartesianAPI_FuncWrap <End> -----------
    // -------- CartesianAPI <End> -----------

    // -------- Real apply class <Start> -----------
    template <size_t Dimension>
    class ViewPositionVector:
            public CartesianAPI<Dimension, false>{
    public:
        ViewPositionVector(): PointData<Dimension>(){}
        ViewPositionVector(const std::initializer_list<double> &v): PointData<Dimension>(v){}

        const double &getDataById(const size_t id) const{
            return PointData<Dimension>::getDataById(id);
        }

    };

    template <size_t Dimension>
    class ComputePositionVector:
            public CartesianAPI<Dimension, true>{
    public:
        ComputePositionVector(): PointData<Dimension>(){}
        ComputePositionVector(const std::initializer_list<double> &v): PointData<Dimension>(v){}

        ComputePositionVector<Dimension>& operator=(const ViewPositionVector<Dimension>& other)
        {
            PointData<Dimension>::operator=(other);
            return *this;
        }

        const double &getDataById(const size_t id) const{
            return PointData<Dimension>::getDataById(id);
        }

        void setDataById(const size_t id, const double &value){
            return PointData<Dimension>::setDataById(id, value);
        }

        void setDataById(const size_t id, double &value){
            return PointData<Dimension>::setDataById(id, value);
        }

    };
    // -------- Real apply class <End> -----------

    template <size_t Dimension>
    class PositionVector{
    public:
        static std::shared_ptr<ViewPositionVector<Dimension>> createViewPoint(){
            std::shared_ptr<ViewPositionVector<Dimension>> re_ptr;
            re_ptr = std::make_shared<ViewPositionVector<Dimension>>();
            return re_ptr;
        }
        static std::shared_ptr<ViewPositionVector<Dimension>> createViewPoint(const std::initializer_list<double> &input_list){
            std::shared_ptr<ViewPositionVector<Dimension>> re_ptr;
            re_ptr = std::make_shared<ViewPositionVector<Dimension>>(input_list);
            return re_ptr;
        }

        static std::shared_ptr<ComputePositionVector<Dimension>> createComputePoint(){
            std::shared_ptr<ComputePositionVector<Dimension>> re_ptr;
            re_ptr = std::make_shared<ComputePositionVector<Dimension>>();
            return re_ptr;
        }
        static std::shared_ptr<ComputePositionVector<Dimension>> createComputePoint(const std::initializer_list<double> &input_list){
            std::shared_ptr<ComputePositionVector<Dimension>> re_ptr;
            re_ptr = std::make_shared<ComputePositionVector<Dimension>>(input_list);
            return re_ptr;
        }

        PositionVector() = delete;
    };
}}




#endif //ARTPDE_POSITION_VECTOR_HPP
