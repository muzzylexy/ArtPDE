#ifndef ARTPDE_GEOMETRIC_UNIT_INFO_H
#define ARTPDE_GEOMETRIC_UNIT_INFO_H

#include <vector>

namespace art_pde{namespace geometry{namespace geometric_tree{

	template <typename DataType> class Cell;
	template <typename DataType> class Face;
	template <typename DataType> class Edge;
	template <typename DataType> class Vertex;

	template<class T>
	class Visitor{
	public:
		virtual void visit(T&) = 0;
	};

	class VisitorBase{
	public:
		virtual ~VisitorBase(){}
	};

	template<class PointType>
	class OperationOnEdge :
		public VisitorBase,
		public Visitor<Edge<PointType>>{
	public:
		virtual void visit(Edge<PointType>& edge) override{
			edge.c_getConnected_Vertex();
			// ...
		}
		std::vector<double>& getNormal(){
			return normal;
		}
	private:
		std::vector<double> normal;
	};

	template<class PointType>
	class pRefineQuadratic :
		public VisitorBase,
		public Visitor<Cell<PointType>>{
	public:
				
		void refine(Cell<PointType>& cell){
			points.clear();
			visit(cell);
			is_refined = true;
		}

		std::vector<std::shared_ptr<PointType>>& getPoint(){
			return points;
		}
		
	private:

		virtual void visit(Cell<PointType>& cell) override{
			double x, y, z;
			int vertex_num;
			for (auto ptr_face : cell.c_getConnected_Face()){
				x = 0.0;
				y = 0.0;
				z = 0.0;
				vertex_num = 0;

				for (auto ptr_vertex : ptr_face->c_getConnected_Vertex()){
					++vertex_num;
					x += ptr_vertex->getPtr_data()->getX();
					y += ptr_vertex->getPtr_data()->getY();
					z += ptr_vertex->getPtr_data()->getZ();
				}
				x /= vertex_num;
				y /= vertex_num;
				z /= vertex_num;

				points.push_back(std::make_shared<PointType>(std::initializer_list<double>{x, y, z}));
			}
		}
		bool is_refined{false};
		std::vector<std::shared_ptr<PointType>> points;
	};
	template<class PointType>
	class FaceNormal :
		public VisitorBase,
		public Visitor<Face<PointType>>{
	public:
		FaceNormal() :normal_(std::vector<double>(3, 0.0)){};
		
		virtual void visit(Face<PointType>& face) override{
			
			for (auto& entries : normal_){
				entries = 0.0;
			}

			auto& faces = face.c_getConnected_Vertex();
			for (auto current = faces.cbegin(); current != faces.cend(); ++current)
			{
				auto next = ((current + 1) != faces.cend()) ? current + 1 : faces.cbegin();

				normal_[0] += ((*current)->getPtr_data()->getY() - (*next)->getPtr_data()->getY())*((*current)->getPtr_data()->getZ() + (*next)->getPtr_data()->getZ());

				normal_[1] += ((*current)->getPtr_data()->getZ() - (*next)->getPtr_data()->getZ())*((*current)->getPtr_data()->getX() + (*next)->getPtr_data()->getX());
		        
				normal_[2] += ((*current)->getPtr_data()->getX() - (*next)->getPtr_data()->getX())*((*current)->getPtr_data()->getY() + (*next)->getPtr_data()->getY());
			}
		}

		std::vector<double>& normal(Face<PointType>& face){
			visit(face);
			return normal_;
		}
	private:
		std::vector<double> normal_;
	};


}}}




#endif