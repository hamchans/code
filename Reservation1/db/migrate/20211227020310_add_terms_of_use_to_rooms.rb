class AddTermsOfUseToRooms < ActiveRecord::Migration[6.1]
  def change
    add_column :rooms, :terms_of_use, :text
  end
end
