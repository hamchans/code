class CreateRooms < ActiveRecord::Migration[6.1]
  def change
    create_table :rooms do |t|
      t.string :name
      t.string :place
      t.integer :number

      t.timestamps
    end
  end
end
