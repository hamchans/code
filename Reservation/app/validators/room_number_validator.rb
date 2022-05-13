class RoomNumberValidator < ActiveModel::EachValidator
  def validate_each(record, attribute, value)
    unless value.to_i % 5 === 0
      record.errors[attribute] << (options[:message] || "収容人数は 5 の倍数で指定してください")
    end
  end
end
