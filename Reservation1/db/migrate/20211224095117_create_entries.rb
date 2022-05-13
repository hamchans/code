class CreateEntries < ActiveRecord::Migration[6.1]
  def change
    create_table :entries do |t|
      t.string :user_name
      t.string :user_email
      t.datetime :reserved_date
      t.float :usage_time
      t.integer :room_id
      t.integer :people

      t.timestamps
    end
    add_index :entries, :user_email, unique: true
  end
end
