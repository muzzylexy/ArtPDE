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
		virtual void visit(Cell<PointType>& cell) override{
			
			for (auto ptr_face : cell.c_getConnected_Face()){
				double x{0.0};
				double y{0.0};
				double z{0.0};
				int vertex_num{0};
				std::cout << "next\n";
				std::cout << *ptr_face << "\n";
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
		
		void refine(std::shared_ptr<Cell<PointType>> cell){
			points.clear();
			visit(*cell);
		}
		std::vector<std::shared_ptr<PointType>>& getPoint(){
			return points;
		}
	private:
		std::vector<std::shared_ptr<PointType>> points;
	};



}}}




#endif