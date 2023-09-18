template <typename Type>
void GooStream<Type>::Reset() noexcept
{
  m_data.Clear();
  m_status = true;
}
