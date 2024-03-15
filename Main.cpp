# include <Siv3D.hpp> // Siv3D v0.6.14
# include "TreeGUI/Node.hpp"

void Main()
{
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });
	Window::Resize(1920, 1009);

	// 自由にノードを作成し、接続する | Create and connect nodes
	// addChildの返り値は追加された子ノード | The return value of addChild is the added child node
	// p()の引数は自身から見て何世代前の祖先かを指定する | The argument of p() specifies how many generations before yourself the ancestor is
	// つまりp(1)で繋いで書くと、兄弟ノードを追加することになる | In other words, if you connect with p(1), you will add a sibling node
	auto root = std::make_shared<TreeGUI::Node>(U"Root");

	root->addChild(U"Child1")->addChild(U"Grandchild1")->addChild(U"Great-grandchild1-1")->p(1)
													   ->addChild(U"Great-grandchild1-2")->p(2)
						     ->addChild(U"Grandchild2")->addChild(U"Great-grandchild2-1")->p(1)
							    					   ->addChild(U"Great-grandchild2-2")->p(3)
		->addChild(U"Child2");

	root->setOpen(true);

	// JSONファイルを読み込む その1 | Load the JSON file 1
	const JSON configJSON = JSON::Load(U"example/json/config.json");
	if (not configJSON)
	{
		throw Error{ U"Failed to load the JSON file" };
	}
	auto configJSONTree = TreeGUI::Node::makeJSONTree(configJSON, U"ConfigJSON");

	// JSONファイルを読み込む その2 | Load the JSON file 2
	const JSON testJSON = JSON::Load(U"example/json/test.json");
	if (not testJSON)
	{
		throw Error{ U"Failed to load the JSON file" };
	}
	auto testJSONTree = TreeGUI::Node::makeJSONTree(testJSON, U"TestJSON");

	// 気象庁の天気のJSONを読み込む | Load the JSON of Tokyo's weather
	const JSON weatherJSON = JSON::Load(U"TokyoWeather.json");
	if (not weatherJSON)
	{
		throw Error{ U"Failed to load the JSON file" };
	}
	auto weatherTree = TreeGUI::Node::makeJSONTree(weatherJSON, U"TokyoWeather");

	// フォルダのツリーを作成する | Create a folder tree
	auto folderTree = TreeGUI::Node::makeFolderTree(U".", U"App");
	folderTree->setOpen(true);

	while (System::Update())
	{
		// ノードを描画する | Draw the node
		root->update({ 50, 50 });
		root->draw();

		configJSONTree->update({ 300, 50 });
		configJSONTree->draw();

		testJSONTree->update({ 600, 50 });
		testJSONTree->draw();

		weatherTree->update({ 900, 50 });
		weatherTree->draw();

		folderTree->update({ 1500, 50 });
		folderTree->draw();
	}
}

