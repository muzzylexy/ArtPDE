//
// Created by Chingkai Chou on 6/2/18.
//

#ifndef ARTPDE_KAVY_GEOMETRIC_READER_ART_PROJECT_HPP
#define ARTPDE_KAVY_GEOMETRIC_READER_ART_PROJECT_HPP

#include <memory>
#include <fstream>
#include <sstream>
#include "../../../Project/art_project.hpp"
#include "../GeometricData/geometric_data_mesh_type.hpp"

namespace art_pde{ namespace geometry {namespace mesh_type {namespace geometric_reader{

	template <typename GeometricReaderType, size_t Dimension>
	class ArtProjectReaderBase{
	public:
		ArtProjectReaderBase(const std::shared_ptr<project::ArtProject> &input_ptr_proj)
			: art_project(input_ptr_proj){};

		virtual bool read() = 0;

	protected:
		using VecPtrVertexType = typename GeometricReaderType::type::VertexType_traits::VecPtrVertexType;
		bool readPosition(VecPtrVertexType &vec_ptr_vertex);

		std::shared_ptr<project::ArtProject> art_project{ nullptr };
	};

	template <typename GeometricReaderType, size_t Dimension> class ArtProjectReader;

	template <typename GeometricReaderType>
	class ArtProjectReader<GeometricReaderType, 2>
		: public ArtProjectReaderBase<GeometricReaderType, 2>{
	public:

		ArtProjectReader<GeometricReaderType, 2>(const std::shared_ptr<project::ArtProject> &input_ptr_proj,
			GeometricReaderType *ptr_data) :
			ArtProjectReaderBase<GeometricReaderType, 2>(input_ptr_proj), ptr_data(ptr_data) {}

		bool read(){
			if (!this->readPosition(ptr_data->getTotalVec_PtrVertex())) return false;
			if (!this->readFace()) return false;
			return true;
		}

	private:
		bool readFace();
		// bool readCell();

		GeometricReaderType *ptr_data;
	};


	template <typename GeometricReaderType>
	class ArtProjectReader<GeometricReaderType, 3>
		: public ArtProjectReaderBase<GeometricReaderType, 3>{
	public:
		ArtProjectReader<GeometricReaderType, 3>(const std::shared_ptr<project::ArtProject> &input_ptr_proj,
			GeometricReaderType *ptr_data) :
			ArtProjectReaderBase<GeometricReaderType, 3>(input_ptr_proj), ptr_data(ptr_data) {}

		bool read(){
			if (!this->readPosition(ptr_data->getTotalVec_PtrVertex())) return false;
			if (!this->readCell()) return false;
			return true;
		}

	private:
		bool readCell();

		GeometricReaderType *ptr_data;
	};

	//#include "../../src/geometric_reader_art_project_impl.cpp"
	template<typename GeometricReaderType, size_t Dimension>
	bool geometric_reader::ArtProjectReaderBase<GeometricReaderType, Dimension>::readPosition(
		typename GeometricReaderType::type::VertexType_traits::VecPtrVertexType &vec_ptr_vertex) {
		std::cout << ">>>> (Start) Loading Position..." << std::endl;
		std::string file_name;
		file_name += this->art_project->getProjectGeometryPath();
		file_name += "position.txt";

		// std::cout << file_name << std::endl;

		using PointType = typename GeometricReaderType::type::PointType;
		using PtrPointType = typename GeometricReaderType::type::PtrPointType;
		using VertexType = typename GeometricReaderType::type::VertexType_traits::VertexType;
		using PtrVertexType = typename GeometricReaderType::type::VertexType_traits::PtrVertexType;

		std::ifstream fs;
		std::string bufferLine;
		double temp_pt[Dimension];
		PtrPointType ptr_point;
		PtrVertexType ptr_vertex;

		fs.open(file_name);
		if (fs.is_open()){
			while (getline(fs, bufferLine)) {

				std::stringstream w(bufferLine);

				for (size_t i = 0; i < Dimension; ++i)
					w >> temp_pt[i];

				ptr_vertex = std::make_shared<VertexType>();
				ptr_point = std::make_shared<PointType>();

				for (size_t i = 0; i < Dimension; ++i){
					ptr_point->setDataById(i, temp_pt[i]);
				}
				ptr_vertex->setPtr_data(ptr_point);

				vec_ptr_vertex.push_back(ptr_vertex);
			}
		}
		else
			return false;

		std::cout << ">>>> ( End ) Loading Position..." << std::endl;
		return true;
	}

	template<typename GeometricReaderType>
	bool geometric_reader::ArtProjectReader<GeometricReaderType, 2>::readFace() {

		using namespace geometric_tree;
		using PointType = typename GeometricReaderType::type::PointType;
		using PtrPointType = typename GeometricReaderType::type::PtrPointType;
		using VertexType = typename GeometricReaderType::type::VertexType_traits::VertexType;
		using PtrVertexType = typename GeometricReaderType::type::VertexType_traits::PtrVertexType;

		std::cout << ">>>> (Start) Loading Connectivity..." << std::endl;
		std::string file_name;
		file_name += this->art_project->getProjectGeometryPath();
		file_name += "connectivity.txt";

		//std::cout << file_name << std::endl;

		std::ifstream fs;
		std::string bufferLine;
		size_t bufferId;

		auto &vec_ptr_vertex = ptr_data->getTotalVec_PtrVertex();
		auto &vec_ptr_face = ptr_data->getTotalVec_PtrFace();
		Face_Factory<PointType> face_factory;

		fs.open(file_name);
		if (fs.is_open()){

			while (getline(fs, bufferLine)) {
				std::stringstream w(bufferLine);

				face_factory.clearVertex();

				while (w >> bufferId){
					face_factory.addVertex(vec_ptr_vertex[bufferId]);
				}

				vec_ptr_face.push_back(
					face_factory.create()
					);
			}
		}
		else
			return false;

		std::cout << ">>>> ( End ) Loading Connectivity..." << std::endl;
		return true;
	}

	template<typename GeometricReaderType>
	bool geometric_reader::ArtProjectReader<GeometricReaderType, 3>::readCell() {
		using namespace geometric_tree;
		using PointType = typename GeometricReaderType::type::PointType;
		using PtrPointType = typename GeometricReaderType::type::PtrPointType;
		using VertexType = typename GeometricReaderType::type::VertexType_traits::VertexType;
		using PtrVertexType = typename GeometricReaderType::type::VertexType_traits::PtrVertexType;

		std::cout << ">>>> (Start) Loading Connectivity..." << std::endl;
		std::string file_name;
		file_name += this->art_project->getProjectGeometryPath();
		file_name += "connectivity.txt";

		//std::cout << file_name << std::endl;

		std::ifstream fs;
		std::string bufferLine;
		size_t bufferId;

		auto &vec_ptr_vertex = this->ptr_data->getTotalVec_PtrVertex();
		auto &vec_ptr_cell = this->ptr_data->getTotalVec_PtrCell();
		Cell_Factory<PointType> cell_factory;

		fs.open(file_name);
		if (fs.is_open()){

			while (getline(fs, bufferLine)) {
				std::stringstream w(bufferLine);

				cell_factory.clearVertex();

				while (w >> bufferId){
					cell_factory.addVertex(vec_ptr_vertex[bufferId]);
				}

				vec_ptr_cell.push_back(
					cell_factory.create()
					);
			}
		}
		else
			return false;

		return true;
}}}}}

#endif //ARTPDE_KAVY_GEOMETRIC_READER_ART_PROJECT_HPP
