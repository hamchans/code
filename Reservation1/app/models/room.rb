class Room < ApplicationRecord
  before_validation :before_document
  validates :name, room_name: true
  validates :number, room_number: true
  validates :name, :place, :number, presence: true
  validates :name, length: {maximum: 30}
  validates :place, inclusion: {in: ['東京', '大阪', '福岡', '札幌', '仙台', '名古屋', '金沢']}
  validates :number, numericality: {greater_than_or_equal_to: 5, less_than_or_equal_to:30}
  validates :terms_of_use, absence: true, unless: :name?
  validates :terms_of_use, length: {maximum: 10}
  
  has_many :entries, dependent: :destroy

  private

  def before_document
    name.strip!
  end
end
