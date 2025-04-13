
import './App.css'


function App() {
  

  return (
    <>
      <div id='navbar' >
        
        <div id='logo'>
          
          {/* <img src="assets/logo.png" alt="" /> */}

        </div>  

        <div id="name">Nishchay's Home Automation </div>


        {/* sign in btn */}
      
        <div id="sign-in">
          <button id='btn'>
            Sign In
          </button>
        </div>

      </div>

      {/* about  */}

      <div className="about">
        <p>Lorem ipsum dolor sit amet, consectetur adipisicing elit. Officia at voluptatum repellendus deserunt fugiat ab sed vero molestiae ut iusto praesentium veniam deleniti impedit vitae, incidunt tenetur nobis nam. Fuga!
        Nemo assumenda dolorum nesciunt soluta odio sequi rerum omnis, architecto quae laudantium vel nobis natus atque, voluptas sint distinctio. Placeat doloribus dolore aliquam fugiat! Earum illum aspernatur sit quaerat ut.
        Corrupti omnis architecto eligendi laborum eveniet doloremque minima, nulla ratione perferendis nisi, nam ipsa necessitatibus inventore doloribus, reprehenderit tenetur. Perferendis quaerat vel optio alias libero error atque sapiente, explicabo a!
        Placeat, provident nobis? Dolorum repudiandae velit cumque laboriosam fugit consequatur? Porro mollitia, totam inventore quam quis, voluptatem fugiat veniam magnam minus cum commodi facilis voluptatibus, quidem sapiente tempora ut impedit.
        Reprehenderit velit enim consequuntur dolore expedita aliquam quas dolorem natus nisi. Atque magni libero quidem fugit, ratione praesentium alias nihil debitis minima totam eum voluptas nemo reiciendis fuga. Dolorum, sint.</p>
      </div>

      {/* input box */}

      <div class="input-box">

        <div class="url">

          Enter IP Address : 

          <input type="text" placeholder='192.102.211' id='text-box' />

          <img src="src/assets/connect.png" alt="Connect" id='connect' />

          {/* <div id='connect'></div> */}

        </div>

          <br />

        {/* buttons */}

        <div class="btn1">
            <img src="assets/" alt="" />
        </div>

      </div>

      {/* footer  */}

      <div id='footer'>
        <p id='ftr'> Home Automation || All Rights reserved </p>
      </div>
      
      
    </>
    
  )
}

export default App
