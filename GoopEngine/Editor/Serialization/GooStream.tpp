template <typename Type>
void GooStream<Type>::Reset() noexcept
{
  m_data.Clear();
  m_elementCount = 0;
  m_status = true;
}
