template<typename CompType>
CompType DeserializeComponent(std::string const& componentData)
{
  throw GE::Debug::Exception<CompType>(Debug::LEVEL_ERROR, ErrMsg("Trying to deserialize unknown component type"));
  return CompType();
}

template<>
GE::Component::Transform DeserializeComponent<GE::Component::Transform>(std::string const& componentData)
{
  Serialization::ComponentWrapper cw{ componentData };
  Serialization::ComponentData data = cw.GetData();

  //read map, manipulate into trans, return
  GE::Component::Transform trans;
  trans.m_pos << data["m_pos"].GetString();
  trans.m_scale << data["m_scale"].GetString();
  trans.m_rot = data["m_rot"].GetDouble();

  return trans;
}

template <>
GE::Component::BoxCollider DeserializeComponent<GE::Component::BoxCollider>(std::string const& componentData)
{
  Serialization::ComponentWrapper cw{ componentData };
  Serialization::ComponentData data = cw.GetData();

  BoxCollider box;
  box.m_width = data["m_width"].GetDouble();
  box.m_height = data["m_height"].GetDouble();
  box.m_min << data["m_min"].GetString();
  box.m_max << data["m_max"].GetString();
  box.m_center << data["m_center"].GetString();
  box.m_mouseCollided = data["m_mouseCollided"].GetBool();

  // may not need to deserialize std::set<BoxCollider*> m_collided

  return box;
}

template<>
GE::Component::Velocity DeserializeComponent<GE::Component::Velocity>(std::string const& componentData)
{
  Serialization::ComponentWrapper cw{ componentData };
  Serialization::ComponentData data = cw.GetData();

  GE::Component::Velocity vel;
  vel.m_acc.x = data["acc_x"].GetDouble();
  vel.m_acc.y = data["acc_y"].GetDouble();
  vel.m_vel.x = data["vel_x"].GetDouble();
  vel.m_vel.y = data["vel_y"].GetDouble();

  return vel;
}

template<>
GE::Gravity DeserializeComponent<GE::Gravity>(std::string const& componentData)
{
  Serialization::ComponentWrapper cw{ componentData };
  Serialization::ComponentData data = cw.GetData();

  Gravity grav;
  grav.m_gravity.x = data["grav_x"].GetDouble();
  grav.m_gravity.y = data["grav_y"].GetDouble();

  // may not need to deserialize std::set<BoxCollider*> m_collided

  return grav;
}

template<>
GE::Component::Sprite DeserializeComponent<GE::Component::Sprite>(std::string const& componentData)
{
  Serialization::ComponentWrapper cw{ componentData };
  Serialization::ComponentData data = cw.GetData();
  auto & gEngine = Graphics::GraphicsEngine::GetInstance();
  Sprite sprite;

  sprite.spriteData.texture = gEngine.textureManager.GetTextureID(data["filename"].GetString());
  return sprite;
}

template<>
GE::Component::SpriteAnim DeserializeComponent<GE::Component::SpriteAnim>(std::string const& componentData)
{
  Serialization::ComponentWrapper cw{ componentData };
  Serialization::ComponentData data = cw.GetData();
  auto& gEngine = Graphics::GraphicsEngine::GetInstance();
  SpriteAnim spriteAnim;

  spriteAnim.animID = gEngine.animManager.GetAnimID(data["name"].GetString());
  return spriteAnim;
}

template<>
GE::Component::Model DeserializeComponent<GE::Component::Model>(std::string const& componentData)
{
    Serialization::ComponentWrapper cw{ componentData };
    Serialization::ComponentData data = cw.GetData();
    Model model;

    model.mdlID = data["id"].GetInt();
    return model;
}