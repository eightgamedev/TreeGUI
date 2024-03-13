# include <Siv3D.hpp> // Siv3D v0.6.14

# include "Node.hpp"
# include "TreeViewer.hpp"

void Main()
{
	// 背景の色を設定する | Set the background color
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });

	// ノードを作成し、接続する | Create and connect nodes
	auto root = std::make_unique<Node>(U"Root");
	root->addChild(U"Child1").addChild(U"Grandchild1").addChildReturnP(U"Great-grandchild1")
														.addChild(U"Great-grandchild2");

	root->addChild(U"Child2");
	root->setOpen(true);

	while (System::Update())
	{
		root->update({ 50, 50 });
		root->draw();
	}
}

